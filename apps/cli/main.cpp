#include "coronan/corona-api_parser.hpp"
#include "coronan/http_client.hpp"

#include <Poco/Net/AcceptCertificateHandler.h>
#include <Poco/Net/Context.h>
#include <Poco/Net/InvalidCertificateHandler.h>
#include <Poco/Net/SSLException.h>
#include <Poco/Net/SSLManager.h>
#include <Poco/SharedPtr.h>
#include <iostream>
#include <lyra/lyra.hpp>

int main(int argc, char* argv[])
{

  // Initialize SSL contect and handler
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

  try
  {
    std::string country = "ch";
    bool help_request = false;
    lyra::cli_parser cli =
        lyra::cli_parser() | lyra::help(help_request) |
        lyra::opt(country, "country")["-c"]["--country"]("Country Code");

    auto const result = cli.parse({argc, argv});
    if (!result)
    {
      std::cerr << "Error in comman line: " << result.errorMessage() << "\n";
      exit(EXIT_FAILURE);
    }

    if (help_request)
    {
      std::cout << cli;
      exit(EXIT_SUCCESS);
    }

    std::string const url = "https://corona-api.com/countries/" + country;

    coronan::HTTPResponse response = coronan::HTTPClient::get(url);

    coronan::CountryObject const data =
        coronan::ApiParser().parse(response.get_response_body());
    std::cout << "\"datetime\", \"confirmed\", \"death\", \"recovered\", "
                 "\"active\"\n";

    std::vector<coronan::CountryObject::timeline_t>::const_iterator it;

    for (it = data.timeline.cbegin(); it != data.timeline.cend(); it++)
    {
      std::cout << (*it).date << ", " << (*it).conf << ", " << (*it).deaths
                << ", " << (*it).recovered << ", " << (*it).active << "\n";
    }
  }
  catch (Poco::Net::SSLException const& ex)
  {
    std::cerr << "SSL Exception: " << ex.displayText() << "\n";
    Poco::Net::uninitializeSSL();
    exit(EXIT_FAILURE);
  }
  Poco::Net::uninitializeSSL();
  exit(EXIT_SUCCESS);
}