#pragma once

#include "coronan/corona-api_datatypes.hpp"

#include <optional>
#include <string>
#include <vector>

namespace coronan {

namespace api_parser {
// Clean Code Note: No need to have a class. Free functions are preferable
// (Scott Meyers  Effective C++ item 23: Prefer Non-member Non-friend Functions
// to Member Functions)
// C++ Core Guidelines: C.4 - Make a function a member only if it needs direct
// access to the representation of a class
/**
 * Parse a json string for country data.
 * @param json json string. Must have the format as described at
 * https://about-corona.net/documentation
 * @return Parsed Covid-19 case data
 */
CountryData parse_country(std::string const& json);

/**
 * Parse a json string for a list of country information
 * @param json json string. Must have the format as described at
 * https://about-corona.net/documentation
 * @return Country list parsed
 */
CountryListObject parse_countries(std::string const& json);
} // namespace api_parser

} // namespace coronan
