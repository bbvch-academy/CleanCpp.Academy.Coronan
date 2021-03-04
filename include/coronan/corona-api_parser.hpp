#pragma once

#include <optional>
#include <string>
#include <vector>

namespace coronan {

/**
 * CountryObject hold the covid-19 data of a single country
 */
struct CountryObject
{
  std::string cn{}; /**< Country name */
  std::string cc{}; /**< Country Code , e.g. ch */
  int32_t pop{};    /**< Country population */

  struct today_t
  {
    std::string date{}; /**< iso date string */
    int32_t deaths{};   /**< todays death cased */
    int32_t conf{};     /**< todays confirmed cases */
  };

  today_t today{}; /**< Todays data when retrieved. New cases of lates date  */

  struct latest_t
  {
    std::string date{};  /**< iso date string (last updated) */
    int32_t deaths{};    /**< latest number of deaths */
    int32_t conf{};      /**< latest number of confirmed cases */
    int32_t recovered{}; /**< latest number of recovered cases */
    int32_t critical{};  /**< latest number of critical cases */
    double dr{};         /**< calculated death rate */
    double rr{};         /**< calculated recovery rate */
    double rdr{};        /**< calculated recovered vs death rate */
    double cpmp{};       /**< calculated cases per millions of
                                                 the population */
  };

  latest_t latest{}; /**< Lates actual cases  */

  struct timeline_t
  {
    std::string date{};      /**< iso date string  */
    int32_t deaths{};        /**< number of deaths */
    int32_t conf{};          /**< number of confirmed cases */
    int32_t active{};        /**< number of current covid-19 cases */
    int32_t recovered{};     /**< number of recovered cases */
    int32_t new_deaths{};    /**< new death since last time data */
    int32_t new_conf{};      /**< new confirmed cases since last time data */
    int32_t new_recovered{}; /**< new recovered cases since last time data */
  };

  std::vector<timeline_t> timeline; /**< array of (daily) data */
};

struct OverviewObject
{
  struct country_t
  {
    std::string name{}; /**< Country name */
    std::string code{}; /**< Country Code , e.g. ch */
  };

  std::vector<country_t> countries; /**< array of available countries */
};

class ApiParser
{
public:
  /**
   * Parse a json.
   * @note Must have the format as described at
   * https://about-corona.net/documentation
   */
  CountryObject parse(std::string const& json);

  /**
   * Parse countries information
   * @note Must have the format as described at
   * https://about-corona.net/documentation
   */
  OverviewObject parse_countries(std::string const& json);
};

} // namespace coronan