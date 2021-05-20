#pragma once

#include <Poco/Net/Context.h>
#include <Poco/Net/InvalidCertificateHandler.h>
#include <Poco/Net/SSLException.h>
#include <Poco/SharedPtr.h>
#include <functional>
#include <memory>

namespace coronan {

using SSLException = Poco::Net::SSLException;

// Clean Code Note: Put the SSL initialisation and uninitialisation in proper
// RAII class
/**
 * An RAII wrapper to initialize & uninitialize the POCO::Net SSL stuff
 * (Poco::Net::initializeSSL())
 */
class SSLClient final
{
public:
  ~SSLClient();
  // Clean Code Note: Make explicite that the returning object must be used,
  // otherwise uninitialze happens
  [[nodiscard]] static std::unique_ptr<SSLClient> create_with_accept_certificate_handler();

private:
  explicit SSLClient(Poco::SharedPtr<Poco::Net::InvalidCertificateHandler> cert_handler,
                     Poco::Net::Context::Ptr net_context);

  void initialize();
  Poco::SharedPtr<Poco::Net::InvalidCertificateHandler> certificate_handler{};
  Poco::Net::Context::Ptr context;
};

} // namespace coronan
