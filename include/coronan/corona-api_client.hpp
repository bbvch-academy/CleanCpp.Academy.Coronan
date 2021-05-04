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

using HTTPClient = HTTPClientT<Poco::Net::HTTPSClientSession,
                               Poco::Net::HTTPRequest, Poco::Net::HTTPResponse>;

// Clean Code Note: Having a dedicated class remived duplication in the two apps
// (cli & Qt aa) using the data Templated to have Compile time polymorphism and
// to allow unittesting without Runtime dependency injection
template <typename ClientT> class CoronaAPIClientT
{
public:
  std::vector<CountryInfo> get_countries() const;
  CountryData get_country_data(std::string const& country_code) const;

private:
  std::string const api_url = corona_api_url;
  SSLClient::SSLClientPtr ssl =
      SSLClient::create_with_accept_certificate_handler();
};

// Clean Code Note: Define a type which is easier to use in applications (no
// template arguments needed).
using CoronaAPIClient = CoronaAPIClientT<HTTPClient>;

// Clean Code Note: Definitions moved down to make the API declaration (above)
// easier to read

template <typename ClientT>
std::vector<CountryInfo> CoronaAPIClientT<ClientT>::get_countries() const
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
        std::string{"\".\n\n Response status: "} + http_response.get_reason() +
        std::string{" ("} + std::to_string(http_response.get_status()) +
        std::string{")."};
    throw HTTPClientException{exception_msg};
  }
}

template <typename ClientT>
CountryData CoronaAPIClientT<ClientT>::get_country_data(
    std::string const& country_code) const
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
        std::string{"\".\n\n Response status: "} + http_response.get_reason() +
        std::string{" ("} + std::to_string(http_response.get_status()) +
        std::string{")."};
    throw HTTPClientException{exception_msg};
  }
}
} // namespace coronan
