#include "coronan/ssl_initializer.hpp"
#include "mainwindow.h"

#include <QDebug>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMessageBox>
#include <iostream>

int main(int argc, char* argv[])
{
  QApplication app(argc, argv);
  QMainWindow window;
  try
  {
    window.show();
    window.setWindowTitle("Co[ro]nan");
    auto const window_width = 900;
    auto const window_height = 600;
    window.resize(window_width, window_height);
    auto* const widget = new CoronanWidget();
    window.setCentralWidget(widget);
    return app.exec(); // NOLINT(readability-static-accessed-through-instance)
  }
  catch (std::exception const& ex)
  {
    const auto error_msg =
        QString{"%1.\n"}.arg(QString::fromStdString(ex.what()));
    qCritical() << error_msg;
    QMessageBox::critical(&window, "Error", error_msg);
    app.exit(EXIT_FAILURE);
  }
}
