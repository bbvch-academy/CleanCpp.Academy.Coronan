#pragma once

#include "coronan/corona-api_parser.hpp"
#include "coronan/http_client.hpp"
#include "coronan/ssl_initializer.hpp"

#include <algorithm>
#include <string>
#include <vector>

namespace coronan {

namespace {
constexpr auto corona_api_url = "https://corona-api.com";
}
template <typename ClientT> class CoronaAPIClientT
{
public:
  // Initialize SSL contect and handler
  /*
  Clean Code Note:
  Everything moved into a class with proper RAII

  using Poco::Net::Context;
  using Poco::Net::InvalidCertificateHandler;
  Poco::Net::Context::Ptr context_ptr = Context::Ptr(
      new Context(Context::TLS_CLIENT_USE, "", "", "",
  Context::VERIFY_RELAXED, 9, false, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH"));

  Poco::SharedPtr<InvalidCertificateHandler> certificate_handler_ptr =
      new Poco::Net::AcceptCertificateHandler(false);
  Poco::Net::initializeSSL();

  Poco::Net::SSLManager::instance().initializeClient(
      nullptr, certificate_handler_ptr, context_ptr);

*/
  std::vector<CountryListObject::CountryInfo> get_countries() const
  {
    auto const countries_url = api_url + std::string{"/countries"};
    if (auto const http_response = ClientT::get(countries_url);
        http_response.get_status() == Poco::Net::HTTPResponse::HTTP_OK)
    {
      return coronan::api_parser::parse_countries(
                 http_response.get_response_body())
          .countries;
    }
    else
    {
      auto const exception_msg =
          std::string{"Error fetching data from url \""} + countries_url +
          std::string{"\".\n\n Response status: "} +
          http_response.get_reason() + std::string{" ("} +
          std::to_string(http_response.get_status()) + std::string{")."};
      throw HTTPClientException{exception_msg};
    }
  }

  CountryData get_country_data(std::string const& country_code) const
  {
    auto const countries_url =
        api_url + std::string{"/countries/"} + country_code;
    if (auto const http_response = ClientT::get(countries_url);
        http_response.get_status() == Poco::Net::HTTPResponse::HTTP_OK)

    {
      return coronan::api_parser::parse_country(
          http_response.get_response_body());
    }
    else
    {
      auto const exception_msg =
          std::string{"Error fetching data from url \""} + countries_url +
          std::string{"\".\n\n Response status: "} +
          http_response.get_reason() + std::string{" ("} +
          std::to_string(http_response.get_status()) + std::string{")."};
      throw HTTPClientException{exception_msg};
    }
  }

private:
  std::string const api_url = corona_api_url;
  SSLInitializer::SSLInitializerPtr ssl =
      SSLInitializer::initialize_with_accept_certificate_handler();
};

using CoronaAPIClient = CoronaAPIClientT<HTTPClient>;

} // namespace coronan
