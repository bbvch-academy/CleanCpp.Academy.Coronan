#include "mainwindow.h"

#include "coronan/corona-api_parser.hpp"
#include "coronan/http_client.hpp"
#include "ui_mainwindow.h"

#include <QDateTime>
#include <QLabel>
#include <QString>
#include <QtCharts/QChartView>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <algorithm>
#include <array>
#include <string>
#include <variant>

CoronanWidget::CoronanWidget(std::string const& api_url, QWidget* parent)
    : QWidget{parent}, m_ui{new Ui_CoronanWidgetForm}, m_url{api_url}
{
  m_ui->setupUi(this);

  // populate country box
  auto http_response = coronan::HTTPClient::get(m_url);
  auto json_object = coronan::ApiParser().parse_countries(http_response.get_response_body());

  auto* countryComboBox = m_ui->countryComboBox;

  coronan::OverviewObject::country_t temp;
  unsigned n = json_object.countries.size();

  // Use Bubble Sort to arrange countries by name
  for (unsigned i = 0; i < n - 1; ++i)
  {
    for (unsigned j = 0; j < n - 1 - i; ++j)
    {
      if (json_object.countries[j].name > json_object.countries[j + 1].name)
      {
        temp = json_object.countries[j];
        json_object.countries[j] = json_object.countries[j + 1];
        json_object.countries[j + 1] = temp;
      }
    }
  }

  std::vector<coronan::OverviewObject::country_t>::iterator it;
  for (it = json_object.countries.begin(); it != json_object.countries.end(); ++it)
  {
    countryComboBox->addItem((*it).name.c_str(), (*it).code.c_str());
  }

  int index = countryComboBox->findData("CH");
  if (index != -1)
  { // -1 for not found
    countryComboBox->setCurrentIndex(index);
  }

  auto country_code = m_ui->countryComboBox->itemData(m_ui->countryComboBox->currentIndex()).toString().toStdString();

  http_response = coronan::HTTPClient::get(m_url + std::string{"/"} + country_code);
  auto const country_data = coronan::ApiParser().parse(http_response.get_response_body());

  // Create chart
  QChart* chart = new QChart{};

  chart->setTitle(QString{"Corona (Covid-19) Cases in "}.append(country_data.cc.c_str()));

  auto const confirmed_serie_name = std::string{"Confirmed"};
  auto const death_serie_name = std::string{"Death"};
  auto const recovered_serie_name = std::string{"Recovered"};

  auto* death_serie = new QLineSeries{};
  death_serie->setName(death_serie_name.c_str());
  auto* confirmed_serie = new QLineSeries{};
  confirmed_serie->setName(confirmed_serie_name.c_str());
  auto* active_serie = new QLineSeries{};
  active_serie->setName("Active");
  auto* recovered_serie = new QLineSeries{};
  recovered_serie->setName(recovered_serie_name.c_str());

  std::array<QLineSeries*, 4> series = {{death_serie, confirmed_serie, active_serie, recovered_serie}};

  for (auto const& data_point : country_data.timeline)
  {
    QDateTime date = QDateTime::fromString(data_point.date.c_str(), "yyyy-MM-ddThh:mm:ss.zZ");
    auto const msecs_since_epoche = date.toMSecsSinceEpoch();
    death_serie->append(QPointF(msecs_since_epoche, data_point.deaths));
    confirmed_serie->append(QPointF(msecs_since_epoche, data_point.conf));
    active_serie->append(QPointF(msecs_since_epoche, data_point.active));
    recovered_serie->append(QPointF(msecs_since_epoche, data_point.recovered));
  }

  for (auto* serie : series)
  {
    chart->addSeries(serie);
  }

  auto* axisX = new QDateTimeAxis{};
  axisX->setFormat("dd/MM  ");
  axisX->setTitleText("Date");
  chart->addAxis(axisX, Qt::AlignBottom);

  auto* axisY = new QValueAxis{};
  axisY->setTitleText("Cases");
  axisY->setLabelFormat("%i  ");

  auto const max_cases = country_data.latest.conf;
  axisY->setRange(0, max_cases);
  axisY->setLinePenColor(confirmed_serie->pen().color());
  axisY->setLabelsColor(confirmed_serie->pen().color());
  axisY->setGridLineColor(confirmed_serie->pen().color());
  chart->addAxis(axisY, Qt::AlignLeft);

  for (auto* serie : series)
  {
    serie->attachAxis(axisX);
    serie->attachAxis(axisY);
  }
  chart->setTheme(QChart::ChartThemeDark);
  chart->legend()->setAlignment(Qt::AlignTop);
  chart->legend()->show();

  m_chartView = new QChartView{chart};

  m_chartView->setRenderHint(QPainter::Antialiasing, true);

  m_ui->gridLayout->addWidget(m_chartView, 2, 1);

  m_ui->overviewTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);

  // create country overview table
  m_ui->overviewTable->setRowCount(7);

  // Population
  m_ui->overviewTable->setItem(0, 0, new QTableWidgetItem{"Population:"});
  m_ui->overviewTable->setItem(0, 1, new QTableWidgetItem{QString::number(country_data.pop)});

  // Population
  m_ui->overviewTable->setItem(1, 0, new QTableWidgetItem{"Confirmed:"});
  m_ui->overviewTable->setItem(1, 1, new QTableWidgetItem{QString::number(country_data.latest.conf)});

  // Death
  m_ui->overviewTable->setItem(2, 0, new QTableWidgetItem{"Death:"});
  m_ui->overviewTable->setItem(2, 1, new QTableWidgetItem{QString::number(country_data.latest.deaths)});

  // Recovered
  m_ui->overviewTable->setItem(3, 0, new QTableWidgetItem{"Recovered:"});
  m_ui->overviewTable->setItem(3, 1, new QTableWidgetItem{QString::number(country_data.latest.recovered)});

  // Critical
  m_ui->overviewTable->setItem(4, 0, new QTableWidgetItem{"Critical:"});
  m_ui->overviewTable->setItem(4, 1, new QTableWidgetItem{QString::number(country_data.latest.critical)});

  // Death rate
  m_ui->overviewTable->setItem(5, 0, new QTableWidgetItem{"Death rate:"});
  m_ui->overviewTable->setItem(5, 1, new QTableWidgetItem{QString::number(country_data.latest.dr)});

  // Critical
  m_ui->overviewTable->setItem(6, 0, new QTableWidgetItem{"Recovery rate:"});
  m_ui->overviewTable->setItem(6, 1, new QTableWidgetItem{QString::number(country_data.latest.rr)});

  // Set the colors from the light theme as default ones
  auto pal = qApp->palette();
  pal.setColor(QPalette::Window, QRgb(0xf0f0f0));
  pal.setColor(QPalette::WindowText, QRgb(0x404044));
  qApp->setPalette(pal);
}

CoronanWidget::~CoronanWidget()
{
  delete m_ui;
}

void CoronanWidget::update_ui()
{
  auto country_code = m_ui->countryComboBox->itemData(m_ui->countryComboBox->currentIndex()).toString().toStdString();
  auto const http_response = coronan::HTTPClient::get(m_url + std::string{"/"} + country_code);
  auto const country_data = coronan::ApiParser().parse(http_response.get_response_body());

  // Create new chart
  QChart* chart = new QChart{};

  chart->setTitle(QString{"Corona (Covid-19) Cases in "}.append(country_data.cc.c_str()));

  auto const confirmed_serie_name = std::string{"Confirmed"};
  auto const death_serie_name = std::string{"Death"};
  auto const recovered_serie_name = std::string{"Recovered"};

  auto* death_serie = new QLineSeries{};
  death_serie->setName(death_serie_name.c_str());
  auto* confirmed_serie = new QLineSeries{};
  confirmed_serie->setName(confirmed_serie_name.c_str());
  auto* active_serie = new QLineSeries{};
  active_serie->setName("Active");
  auto* recovered_serie = new QLineSeries{};
  recovered_serie->setName(recovered_serie_name.c_str());

  std::array<QLineSeries*, 4> series = {{death_serie, confirmed_serie, active_serie, recovered_serie}};

  for (auto const& data_point : country_data.timeline)
  {
    QDateTime date = QDateTime::fromString(data_point.date.c_str(), "yyyy-MM-ddThh:mm:ss.zZ");
    auto const msecs_since_epoche = date.toMSecsSinceEpoch();
    death_serie->append(QPointF(msecs_since_epoche, data_point.deaths));
    confirmed_serie->append(QPointF(msecs_since_epoche, data_point.conf));
    active_serie->append(QPointF(msecs_since_epoche, data_point.active));
    recovered_serie->append(QPointF(msecs_since_epoche, data_point.recovered));
  }

  for (auto* serie : series)
  {
    chart->addSeries(serie);
  }

  auto* axisX = new QDateTimeAxis{};
  axisX->setFormat("dd/MM  ");
  axisX->setTitleText("Date");
  chart->addAxis(axisX, Qt::AlignBottom);

  auto* axisY = new QValueAxis{};
  axisY->setTitleText("Cases");
  axisY->setLabelFormat("%i  ");

  auto const max_cases = country_data.latest.conf;
  axisY->setRange(0, max_cases);
  axisY->setLinePenColor(confirmed_serie->pen().color());
  axisY->setLabelsColor(confirmed_serie->pen().color());
  axisY->setGridLineColor(confirmed_serie->pen().color());
  chart->addAxis(axisY, Qt::AlignLeft);

  for (auto* serie : series)
  {
    serie->attachAxis(axisX);
    serie->attachAxis(axisY);
  }
  chart->setTheme(QChart::ChartThemeDark);
  chart->legend()->setAlignment(Qt::AlignTop);
  chart->legend()->show();

  QChartView* new_chartView = new QChartView{chart};

  new_chartView->setRenderHint(QPainter::Antialiasing, true);
  auto* old_layout = m_ui->gridLayout->replaceWidget(m_chartView, new_chartView);
  delete old_layout;
  m_chartView = new_chartView;

  // update country overview table
  m_ui->overviewTable->setRowCount(7);

  // Population
  m_ui->overviewTable->setItem(0, 0, new QTableWidgetItem{"Population:"});
  m_ui->overviewTable->setItem(0, 1, new QTableWidgetItem{QString::number(country_data.pop)});

  // Population
  m_ui->overviewTable->setItem(1, 0, new QTableWidgetItem{"Confirmed:"});
  m_ui->overviewTable->setItem(1, 1, new QTableWidgetItem{QString::number(country_data.latest.conf)});

  // Death
  m_ui->overviewTable->setItem(2, 0, new QTableWidgetItem{"Death:"});
  m_ui->overviewTable->setItem(2, 1, new QTableWidgetItem{QString::number(country_data.latest.deaths)});

  // Recovered
  m_ui->overviewTable->setItem(3, 0, new QTableWidgetItem{"Recovered:"});
  m_ui->overviewTable->setItem(3, 1, new QTableWidgetItem{QString::number(country_data.latest.recovered)});

  // Critical
  m_ui->overviewTable->setItem(4, 0, new QTableWidgetItem{"Critical:"});
  m_ui->overviewTable->setItem(4, 1, new QTableWidgetItem{QString::number(country_data.latest.critical)});

  // Death rate
  m_ui->overviewTable->setItem(5, 0, new QTableWidgetItem{"Death rate:"});
  m_ui->overviewTable->setItem(5, 1, new QTableWidgetItem{QString::number(country_data.latest.dr)});

  // Critical
  m_ui->overviewTable->setItem(6, 0, new QTableWidgetItem{"Recovery rate:"});
  m_ui->overviewTable->setItem(6, 1, new QTableWidgetItem{QString::number(country_data.latest.rr)});
}
