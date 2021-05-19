#include "coronan/http_client.hpp"

namespace coronan {

HTTPClientException::HTTPClientException(std::string const& exception_msg) : msg{exception_msg}
{
}

char const* HTTPClientException::what() const noexcept
{
  return msg.c_str();
}

HTTPResponse::HTTPResponse(Poco::Net::HTTPResponse const& response, std::string const& response_body)
    : response_{response}, response_body_{response_body}
{
}

Poco::Net::HTTPResponse::HTTPStatus
// cppcheck-suppress unusedFunction
HTTPResponse::status() const
{
  return response_.getStatus();
}

// cppcheck-suppress unusedFunction
std::string HTTPResponse::reason() const
{
  return response_.getReason();
}

// cppcheck-suppress unusedFunction
std::string HTTPResponse::response_body() const
{
  return response_body_;
}

} // namespace coronan
