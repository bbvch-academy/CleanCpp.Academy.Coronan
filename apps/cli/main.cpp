#include "coronan/corona-api_parser.hpp"
#include "coronan/http_client.hpp"
#include "coronan/ssl_initializer.hpp"

#include <iostream>
#include <lyra/lyra.hpp>

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

    auto const result = cli.parse({argc, argv});
    if (!result)
    {
      std::cerr << "Error in comman line: " << result.errorMessage() << "\n";
      // Poco::Net::uninitializeSSL();
      // Clean Code Note: Was forgotten
      exit(EXIT_FAILURE);
    }

    if (help_request)
    {
      std::cout << cli;
      // Poco::Net::uninitializeSSL();
      // Clean Code Note: Was forgotten
      exit(EXIT_SUCCESS);
    }

    // Clean Code Note: With auto the typ must not be written explicitly
    auto const url = "https://corona-api.com/countries/" + country;

    auto response = coronan::HTTPClient::get(url);

    auto const& data =
        coronan::api_parser::parse_country(response.get_response_body());
    std::cout << "\"datetime\", \"confirmed\", \"death\", \"recovered\", "
                 "\"active\"\n";

    // Clean Code Note: Range based for loop is much less verbose and clearer to
    // understand

    auto const to_string = [](auto val) {
      return val.has_value() ? std::to_string(val.value()) : "--";
    };

    for (auto const& data_point : data.timeline)
    {
      std::cout << data_point.date << ", " << to_string(data_point.confirmed)
                << ", " << to_string(data_point.deaths) << ", "
                << to_string(data_point.recovered) << ", "
                << to_string(data_point.active) << "\n";
    }
  }
  catch (coronan::SSLException const& ex)
  {
    std::cerr << "SSL Exception: " << ex.displayText() << "\n";
    exit(EXIT_FAILURE);
  }
  // Poco::Net::uninitializeSSL(); called in destructor of the static
  // ssl_initializer_handler
  exit(EXIT_SUCCESS);
}
