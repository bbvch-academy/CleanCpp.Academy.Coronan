#include "coronan/ssl_initializer.hpp"
#include "mainwindow.h"

#include <QDebug>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
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
  try
  {

    QApplication a(argc, argv);
    QMainWindow window;
    CoronanWidget* widget = new CoronanWidget(api_url);
    window.setCentralWidget(widget);
    window.resize(900, 600);
    window.show();
    return a.exec();
  }
  catch (coronan::SSLException const& ex)
  {
    qCritical() << "SSL Exception: " << ex.displayText().c_str() << "\n";
    exit(EXIT_FAILURE);
  }
}
