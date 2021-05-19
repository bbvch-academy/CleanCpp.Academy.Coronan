#pragma once

#include "coronan/corona-api_parser.hpp"
#include "coronan/http_client.hpp"
#include "coronan/ssl_client.hpp"

#include <algorithm>
#include <string>
#include <vector>

namespace coronan {

namespace {
constexpr auto corona_api_url = "https://corona-api.com";
}

using HTTPClient = HTTPClientT<Poco::Net::HTTPSClientSession, Poco::Net::HTTPRequest, Poco::Net::HTTPResponse>;

// Clean Code Note: Having a dedicated class remived duplication in the two apps
// (cli & Qt aa) using the data Templated to have Compile time polymorphism and
// to allow unittesting without Runtime dependency injection
template <typename ClientType>
class CoronaAPIClientType
{
public:
  std::vector<CountryInfo> request_countries() const;
  CountryData request_country_data(std::string const& country_code) const;

private:
  std::string const api_url = corona_api_url;
  SSLClient ssl = SSLClient::create_with_accept_certificate_handler();
};

// Clean Code Note: Define a type which is easier to use in applications (no
// template arguments needed).
using CoronaAPIClient = CoronaAPIClientType<HTTPClient>;

// Clean Code Note: Definitions moved down to make the API declaration (above)
// easier to read

template <typename ClientType>
std::vector<CountryInfo> CoronaAPIClientType<ClientType>::request_countries() const
{
  auto const countries_url = api_url + std::string{"/countries"};
  auto const http_response = ClientType::get(countries_url);
  if (http_response.status() == Poco::Net::HTTPResponse::HTTP_OK)
  {
    return coronan::api_parser::parse_countries(http_response.response_body()).countries;
  }
  else
  {
    auto const exception_msg = std::string{"Error fetching data from url \""} + countries_url +
                               std::string{"\".\n\n Response status: "} + http_response.reason() + std::string{" ("} +
                               std::to_string(http_response.status()) + std::string{")."};
    throw HTTPClientException{exception_msg};
  }
}

template <typename ClientType>
CountryData CoronaAPIClientType<ClientType>::request_country_data(std::string const& country_code) const
{
  auto const country_url = api_url + std::string{"/countries/"} + country_code;
  auto const http_response = ClientType::get(country_url);
  if (http_response.status() == Poco::Net::HTTPResponse::HTTP_OK)

  {
    return coronan::api_parser::parse_country(http_response.response_body());
  }
  else
  {
    auto const exception_msg = std::string{"Error fetching data from url \""} + country_url +
                               std::string{"\".\n\n Response status: "} + http_response.reason() + std::string{" ("} +
                               std::to_string(http_response.status()) + std::string{")."};
    throw HTTPClientException{exception_msg};
  }
}

} // namespace coronan
