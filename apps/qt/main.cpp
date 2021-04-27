#include "mainwindow.h"

#include <Poco/Net/AcceptCertificateHandler.h>
#include <Poco/Net/Context.h>
#include <Poco/Net/InvalidCertificateHandler.h>
#include <Poco/Net/SSLException.h>
#include <Poco/Net/SSLManager.h>
#include <Poco/SharedPtr.h>
#include <QDebug>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <iostream>

constexpr auto api_url = "https://corona-api.com/countries";

int main(int argc, char* argv[])
{

  // Initialize SSL contect and handler
  using Poco::Net::Context;
  using Poco::Net::InvalidCertificateHandler;

 #ifdef WIN32
  Poco::Net::Context::Ptr context_ptr = Context::Ptr(
      new Context{Context::TLS_CLIENT_USE, "", Context::VERIFY_RELAXED});

#else
  Poco::Net::Context::Ptr context_ptr = Context::Ptr(
      new Context(Context::TLS_CLIENT_USE, "", "", "", Context::VERIFY_RELAXED,
                  9, false, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH"));
#endif

  Poco::SharedPtr<InvalidCertificateHandler> certificate_handler_ptr =
      new Poco::Net::AcceptCertificateHandler(false);
  Poco::Net::initializeSSL();

  Poco::Net::SSLManager::instance().initializeClient(
      nullptr, certificate_handler_ptr, context_ptr);
  int result = 0;

  try
  {

    QApplication a(argc, argv);
    QMainWindow window;
    CoronanWidget* widget = new CoronanWidget(api_url);
    window.setCentralWidget(widget);
    window.resize(900, 600);
    window.show();
    result = a.exec();
  }
  catch (Poco::Net::SSLException const& ex)
  {
    qCritical() << "SSL Exception: " << ex.displayText().c_str() << "\n";
    exit(EXIT_FAILURE);
  }
  Poco::Net::uninitializeSSL();
  return result;
}
