#include "coronan/ssl_initializer.hpp"
#include "mainwindow.h"

#include <QDebug>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMessageBox>
#include <iostream>

constexpr auto api_url = "https://corona-api.com/countries";

int main(int argc, char* argv[])
{
  /*
Clean Code Note:
Everything moved into a class with proper RAII

using Poco::Net::Context;
using Poco::Net::InvalidCertificateHandler;
Poco::Net::Context::Ptr context_ptr = Context::Ptr(
    new Context(Context::TLS_CLIENT_USE, "", "", "", Context::VERIFY_RELAXED,
                9, false, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH"));

Poco::SharedPtr<InvalidCertificateHandler> certificate_handler_ptr =
    new Poco::Net::AcceptCertificateHandler(false);
Poco::Net::initializeSSL();

Poco::Net::SSLManager::instance().initializeClient(
    nullptr, certificate_handler_ptr, context_ptr);

*/
  static auto const ssl_initializer_handler =
      coronan::SSLInitializer::initialize_with_accept_certificate_handler();
  QApplication app(argc, argv);
  QMainWindow window;
  window.show();
  window.setWindowTitle("Co[ro]nan");
  try
  {
    auto const window_width = 900;
    auto const window_height = 600;
    window.resize(window_width, window_height);
    auto* const widget = new CoronanWidget(api_url);
    window.setCentralWidget(widget);
    return app.exec(); // NOLINT(readability-static-accessed-through-instance)
  }
  catch (coronan::SSLException const& ex)
  {
    const auto error_msg = QString{"SSL Exception: %1.\n"}.arg(
        QString::fromStdString(ex.displayText()));
    qCritical() << error_msg;
    QMessageBox::critical(&window, "Error", error_msg);
    app.exit(EXIT_FAILURE);
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
