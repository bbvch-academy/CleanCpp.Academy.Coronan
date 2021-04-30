#include "coronan/corona-api_parser.hpp"
#include "coronan/http_client.hpp"
#include "coronan/ssl_initializer.hpp"

#include <fmt/core.h>
#include <lyra/lyra.hpp>
#include <sstream>

int main(int argc, char* argv[])
{
  // Initialize SSL contect and handler
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
    std::string country = "ch";
    bool help_request = false;
    auto cli = lyra::cli_parser() | lyra::help(help_request) |
               lyra::opt(country, "country")["-c"]["--country"]("Country Code");

    std::stringstream usage;
    usage << cli;

    auto const result = cli.parse({argc, argv});
    if (!result)
    {
      fmt::print(stderr, "Error in comman line: {}\n", result.errorMessage());
      fmt::print("{}\n", usage.str());
      // Poco::Net::uninitializeSSL();
      // Clean Code Note: Was forgotten
      exit(EXIT_FAILURE);
    }

    if (help_request)
    {
      fmt::print("{}\n", usage.str());
      // Poco::Net::uninitializeSSL();
      // Clean Code Note: Was forgotten
      exit(EXIT_SUCCESS);
    }

    // Clean Code Note: With auto the typ must not be written explicitly
    auto const url = std::string{"https://corona-api.com/countries/"} + country;

    auto const response = coronan::HTTPClient::get(url);

    if (response.get_status() == Poco::Net::HTTPResponse::HTTP_OK)
    {

      auto const& data =
          coronan::api_parser::parse_country(response.get_response_body());
      fmt::print("\"datetime\", \"confirmed\", \"death\", \"recovered\", "
                 "\"active\"\n");

      // Clean Code Note: Range based for loop is much less verbose and clearer
      // to understand
      for (auto const& data_point : data.timeline)
      {
        fmt::print("{date}, {confirmed}, {deaths}, {recovered}, {active}\n",
                   fmt::arg("date", data_point.date),
                   fmt::arg("confirmed", data_point.confirmed.value_or(0)),
                   fmt::arg("deaths", data_point.deaths.value_or(0)),
                   fmt::arg("recovered", data_point.recovered.value_or(0)),
                   fmt::arg("active", data_point.active.value_or(0)));
      }
    }
    else
    {
      fmt::print("Error fetching {url}. Response Status: {status}.\n",
                 fmt::arg("url", url),
                 fmt::arg("status", response.get_status()));
    }
  }
  catch (coronan::SSLException const& ex)
  {

    fmt::print(stderr, "SSL Exception: {}\n", ex.displayText());
    exit(EXIT_FAILURE);
  }
  // Poco::Net::uninitializeSSL(); called in destructor of the static
  // ssl_initializer_handler
  exit(EXIT_SUCCESS);
}
