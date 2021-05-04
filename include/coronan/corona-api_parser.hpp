#pragma once

#include <optional>
#include <string>
#include <vector>

namespace coronan {

/**
 * CountryData hold the covid-19 data of a single country
 */
struct CountryData
{
  // Clean Code Note: Write the understandble names use abbreviations only if
  // commonly known
  std::string name{};         /**< Country name */
  std::string country_code{}; /**< Country Code , e.g. ch */
  // Clean Code Note: By using the optional type we can handle the case with
  // no value properly
  std::optional<uint32_t> population{}; /**< Country population */

  // Clean Code Note: For consistency types should be named with Uppercase
  // CamelCase
  struct TodayData
  {
    std::string date{};                  /**< iso date string */
    std::optional<uint32_t> deaths{};    /**< todays death cased */
    std::optional<uint32_t> confirmed{}; /**< todays confirmed cases */
  };

  TodayData
      today{}; /**< TodayDatas data when retrieved. New cases of lates date  */

  struct LatestData
  {
    std::string date{};               /**< iso date string (last updated) */
    std::optional<uint32_t> deaths{}; /**< latest number of deaths */
    std::optional<uint32_t>
        confirmed{}; /**< latest number of confirmed cases */
    std::optional<uint32_t>
        recovered{};                    /**< latest number of recovered cases */
    std::optional<uint32_t> critical{}; /**< latest number of critical cases */
    std::optional<double> death_rate{}; /**< calculated death rate */
    std::optional<double> recovery_rate{}; /**< calculated recovery rate */
    std::optional<double>
        recovered_vs_death_ratio{}; /**< calculated recovered vs death rate */
    std::optional<uint32_t>
        cases_per_million_population{}; /**< calculated cases
                       per millions of the population */
  };

  LatestData latest{}; /**< Lates actual cases  */

  struct TimelineData
  {
    std::string date{};                  /**< iso date string  */
    std::optional<uint32_t> deaths{};    /**< number of deaths */
    std::optional<uint32_t> confirmed{}; /**< number of confirmed cases */
    std::optional<uint32_t> active{}; /**< number of current covid-19 cases */
    std::optional<uint32_t> recovered{};  /**< number of recovered cases */
    std::optional<uint32_t> new_deaths{}; /**< new death since last time data */
    std::optional<uint32_t>
        new_confirmed{}; /**< new confirmed cases since last time data */
    std::optional<uint32_t>
        new_recovered{}; /**< new recovered cases since last time data */
  };

  std::vector<TimelineData> timeline{}; /**< array of (daily) data */
};

struct CountryListObject
{
  struct CountryInfo
  {
    std::string name{}; /**< Country name */
    std::string code{}; /**< Country Code , e.g. ch */
  };

  std::vector<CountryInfo> countries{}; /**< array of available countries */
};

namespace api_parser {
// Clean Code Note: No need to have class. free functions are preferable
// (Scott Meyers  Effective C++ item 23: Prefer Non-member Non-friend Functions
// to Member Functions)
// C++ Core Guidelines: C.4 - Make a function a member only if it needs direct
// access to the representation of a class
/**
 * Parse a json.
 * @note Must have the format as described at
 * https://about-corona.net/documentation
 */
CountryData parse_country(std::string const& json);

/**
 * Parse countries information
 * @note Must have the format as described at
 * https://about-corona.net/documentation
 */
CountryListObject parse_countries(std::string const& json);
} // namespace api_parser

} // namespace coronan
