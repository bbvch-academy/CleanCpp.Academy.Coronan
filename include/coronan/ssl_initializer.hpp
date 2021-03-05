#pragma once

#include <Poco/Net/Context.h>
#include <Poco/Net/InvalidCertificateHandler.h>
#include <Poco/Net/SSLException.h>
#include <Poco/SharedPtr.h>
#include <memory>

using Poco::Net::InvalidCertificateHandler;

namespace coronan {

using SSLException = Poco::Net::SSLException;

// Clean Code Note: Put the SSL initialisation adn uninitialize in proper RAII
// class
class SSLInitializer
{
public:
  explicit SSLInitializer(InvalidCertificateHandler* certificate_handler,
                          Poco::Net::Context::Ptr context);
  void initialize();

  ~SSLInitializer();

  // Clean Code Note: Make explicite that the returning object mus be used,
  // otherwise uninitialze happens
  [[nodiscard]] static std::unique_ptr<SSLInitializer>
  initialize_with_accept_certificate_handler();

private:
  Poco::SharedPtr<InvalidCertificateHandler> certificate_handler_ptr;
  Poco::Net::Context::Ptr context_ptr;
};

} // namespace coronan