#pragma once

#include <Poco/Net/Context.h>

namespace coronan {
namespace ssl_context {

// Clean Code Note: The window/Linux difference of the the Poco API is handled by compiling the right source file
// ssl_context-linux.cpp or ssl_context-win.cpp in the CMakeLists.txt
Poco::Net::Context::Ptr create_verify_relaxed_context();

} // namespace ssl_context
} // namespace coronan
