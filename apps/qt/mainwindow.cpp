#include "mainwindow.h"

#include "coronan/corona-api_client.hpp"
#include "ui_mainwindow.h"

#include <QDateTime>
#include <QDebug>
#include <QLabel>
#include <QString>
#include <QtCharts/QChartView>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMessageBox>
#include <algorithm>
#include <array>
#include <string>
#include <variant>

namespace {
// Clean Code Note:
// The use of helper methods (lambdas) makes code more readable and reusable at
// different places Creating and updating the country overview table is the same
// --> extract to function

template <class... Ts> struct overloaded : Ts...
{
  using Ts::operator()...;
};
template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>; // not needed as of C++20

constexpr auto update_country_overview_table = [](auto* table,
                                                  auto const& country_data) {
  auto const label_col_index = 0;
  auto const value_col_index = 1;

  using VariantT = std::variant<std::optional<uint32_t>, std::optional<double>>;
  using CaptionValuePair = std::pair<QString, VariantT>;
  constexpr auto no_table_entries = 7;
  std::array<CaptionValuePair, no_table_entries> const overview_table_entries =
      {{std::make_pair("Population:", country_data.population),
        std::make_pair("Confirmed:", country_data.latest.confirmed),
        std::make_pair("Death:", country_data.latest.deaths),
        std::make_pair("Recovered:", country_data.latest.recovered),
        std::make_pair("Critical:", country_data.latest.critical),
        std::make_pair("Death rate:", country_data.latest.death_rate),
        std::make_pair("Recovery rate::", country_data.latest.recovery_rate)}};

  table->setRowCount(no_table_entries);

  auto row_index = 0;
  for (auto const& pair : overview_table_entries)
  {
    auto* const label_widget = new QTableWidgetItem{pair.first};
    table->setItem(row_index, label_col_index, label_widget);
    // Clean Code Note: variant with visit functions allows to handle the empty
    // (optional) type without if statements
    auto const value_str = std::visit(
        overloaded{[](auto const& arg) {
          return arg.has_value() ? QString::number(arg.value()) : QString{"--"};
        }},
        pair.second);

    auto* const value_widget = new QTableWidgetItem{value_str};
    table->setItem(row_index, value_col_index, value_widget);

    row_index++;
  }
};

constexpr auto create_line_chart =
    [](coronan::CountryObject const& country_data) {
      auto* const chart = new QChart{};

      chart->setTitle(QString{"Corona (Covid-19) Cases in "}.append(
          country_data.name.c_str()));

      auto* const death_serie = new QLineSeries{};
      death_serie->setName("Death");
      auto* const confirmed_serie = new QLineSeries{};
      confirmed_serie->setName("Confirmed");
      auto* const active_serie = new QLineSeries{};
      active_serie->setName("Active");
      auto* const recovered_serie = new QLineSeries{};
      recovered_serie->setName("Recovered");

      for (auto const& data_point : country_data.timeline)
      {
        QDateTime const date = QDateTime::fromString(data_point.date.c_str(),
                                                     "yyyy-MM-ddThh:mm:ss.zZ");
        auto const msecs_since_epoche =
            static_cast<double>(date.toMSecsSinceEpoch());

        auto const append_value_to_serie =
            [msecs_since_epoche](auto const& value, auto* const serie) {
              if (value.has_value())
              {
                serie->append(QPointF(msecs_since_epoche, value.value()));
              }
            };

        append_value_to_serie(data_point.deaths, death_serie);
        append_value_to_serie(data_point.confirmed, confirmed_serie);
        append_value_to_serie(data_point.active, active_serie);
        append_value_to_serie(data_point.recovered, recovered_serie);
      }

      auto* const axisX = new QDateTimeAxis{};
      axisX->setFormat("dd/MM  ");
      axisX->setTitleText("Date");
      chart->addAxis(axisX, Qt::AlignBottom);

      auto* const axisY = new QValueAxis{};
      axisY->setTitleText("Cases");
      axisY->setLabelFormat("%i  ");

      auto const max_cases = country_data.latest.confirmed.value_or(0);
      axisY->setRange(0, max_cases);
      axisY->setLinePenColor(confirmed_serie->pen().color());
      axisY->setLabelsColor(confirmed_serie->pen().color());
      axisY->setGridLineColor(confirmed_serie->pen().color());
      chart->addAxis(axisY, Qt::AlignLeft);

      for (auto* const serie : std::vector<QLineSeries*>{
               death_serie, confirmed_serie, active_serie, recovered_serie})
      {
        chart->addSeries(serie);
        serie->attachAxis(axisX);
        serie->attachAxis(axisY);
      }

      chart->setTheme(QChart::ChartThemeDark);
      chart->legend()->setAlignment(Qt::AlignTop);
      chart->legend()->show();

      return chart;
    };

constexpr auto create_chart_view = [](auto const& country_data) {
  auto* const chartView = new QChartView{create_line_chart(country_data)};
  chartView->setRenderHint(QPainter::Antialiasing, true);
  return chartView;
};

} // namespace

CoronanWidget::CoronanWidget(QWidget* parent)
    : QWidget{parent}, ui{new Ui_CoronanWidgetForm}
{
  ui->setupUi(this);

  populate_country_box();
  update_ui();

  QObject::connect(ui->countryComboBox, SIGNAL(currentIndexChanged(int)), this,
                   SLOT(update_ui()));
}

CoronanWidget::~CoronanWidget() { delete ui; }

void CoronanWidget::populate_country_box()
{
  auto* country_combo = ui->countryComboBox;
  auto countries = coronan::CoronaAPIClient{}.get_countries();

  std::sort(begin(countries), end(countries),
            [](auto const& a, auto const& b) { return a.name < b.name; });

  for (auto const& country : countries)
  {
    country_combo->addItem(country.name.c_str(), country.code.c_str());
  }
  if (int const index = country_combo->findData("CH"); index != -1)
  { // -1 for not found
    country_combo->setCurrentIndex(index);
  }
}

coronan::CountryObject
CoronanWidget::get_country_data(std::string const& country_code)
{
  try
  {
    return coronan::CoronaAPIClient{}.get_country_data(country_code);
  }
  catch (coronan::SSLException const& ex)
  {
    QMessageBox::warning(this, "SSL Exception",
                         QString::fromStdString(ex.displayText()));
  }
  catch (coronan::HTTPClientException const& ex)
  {
    QMessageBox::warning(this, "HTTP Client Exception",
                         QString::fromStdString(ex.what()));
  }
  catch (std::exception const& ex)
  {
    QMessageBox::warning(this, "Exception", QString::fromStdString(ex.what()));
  }
  return {};
}

void CoronanWidget::update_ui()
{
  auto country_code =
      ui->countryComboBox->itemData(ui->countryComboBox->currentIndex())
          .toString();
  auto const country_data = get_country_data(country_code.toStdString());
  auto* const new_chartView = create_chart_view(country_data);
  if (chartView == nullptr)
  {
    ui->gridLayout->addWidget(new_chartView, 2, 1);

    ui->overviewTable->horizontalHeader()->setSectionResizeMode(
        0, QHeaderView::ResizeToContents);
  }
  else
  {
    auto* old_layout = ui->gridLayout->replaceWidget(chartView, new_chartView);
    delete old_layout;
  }
  chartView = new_chartView;
  update_country_overview_table(ui->overviewTable, country_data);
}
