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
/**
 * An RAII wrapper to initialize & uninitialize the POCO::Net SSL stuff
 * (Poco::Net::initializeSSL())
 */
class SSLInitializer
{
public:
  ~SSLInitializer();

  // Clean Code Note: Make explicite that the returning object must be used,
  // otherwise uninitialze happens
  [[nodiscard]] static std::unique_ptr<SSLInitializer>
  initialize_with_accept_certificate_handler();

private:
  explicit SSLInitializer(
      Poco::SharedPtr<InvalidCertificateHandler> certificate_handler,
      Poco::Net::Context::Ptr context);

  void initialize_client();
  Poco::SharedPtr<InvalidCertificateHandler> certificate_handler_ptr{};
  Poco::Net::Context::Ptr context_ptr;
};

} // namespace coronan
