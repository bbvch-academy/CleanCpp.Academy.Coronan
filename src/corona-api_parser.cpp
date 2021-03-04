#include "coronan/corona-api_parser.hpp"

#include "rapidjson/document.h"

#include <algorithm>

namespace coronan {

CountryObject ApiParser::parse(std::string const& json)
{
  rapidjson::Document document;
  document.Parse<rapidjson::kParseFullPrecisionFlag>(json.c_str());
  auto country_object = CountryObject{};
  auto const country_data_object = document["data"].GetObject();
  country_object.cn = country_data_object["name"].GetString();
  country_object.cc = country_data_object["code"].GetString();
  country_object.pop = country_data_object["population"].GetInt();

  country_object.today = CountryObject::today_t();
  country_object.today.deaths =
      country_data_object["today"].GetObject()["deaths"].GetInt();
  country_object.today.conf =
      country_data_object["today"].GetObject()["confirmed"].GetInt();
  auto const current_date = country_data_object["updated_at"].GetString();
  country_object.today.date = current_date;
  country_object.latest = CountryObject::latest_t();
  country_object.latest.deaths =
      country_data_object["latest_data"].GetObject()["deaths"].GetInt();
  country_object.latest.conf =
      country_data_object["latest_data"].GetObject()["confirmed"].GetInt();
  country_object.latest.recovered =
      country_data_object["latest_data"].GetObject()["recovered"].GetInt();
  country_object.latest.critical =
      country_data_object["latest_data"].GetObject()["critical"].GetInt();
  auto const calculated =
      country_data_object["latest_data"].GetObject()["calculated"].GetObject();
  if (calculated["death_rate"].IsNumber())
  {
    country_object.latest.dr = calculated["death_rate"].GetDouble();
  }
  else
  {
    country_object.latest.dr = 0.0;
  }
  if (calculated["recovery_rate"].IsNumber())
  {
    country_object.latest.rr = calculated["recovery_rate"].GetDouble();
  }
  else
  {
    country_object.latest.rr = 0.0;
  }
  if (calculated["recovered_vs_death_ratio"].IsNumber())
  {
    country_object.latest.rdr =
        calculated["recovered_vs_death_ratio"].GetDouble();
  }
  else
  {
    country_object.latest.rdr = 0.0;
  }
  if (calculated["cases_per_million_population"].IsNumber())
  {
    country_object.latest.cpmp =
        calculated["cases_per_million_population"].GetDouble();
  }
  else
  {
    country_object.latest.cpmp = 0.0;
  }
  country_object.latest.date = current_date;
  country_object.timeline = std::vector<CountryObject::timeline_t>();

  for (auto& data_point : country_data_object["timeline"].GetArray())
  {
    CountryObject::timeline_t timepoint;
    timepoint.date = data_point["updated_at"].GetString();
    timepoint.deaths = data_point["deaths"].GetInt();
    timepoint.conf = data_point["confirmed"].GetInt();
    timepoint.recovered = data_point["recovered"].GetInt();
    timepoint.active = data_point["active"].GetInt();
    timepoint.new_conf = data_point["new_confirmed"].GetInt();
    timepoint.new_recovered = data_point["new_recovered"].GetInt();
    timepoint.new_deaths = data_point["new_deaths"].GetInt();
    country_object.timeline.push_back(timepoint);
  }
  return country_object;
}

OverviewObject ApiParser::parse_countries(std::string const& json)
{
  rapidjson::Document document;
  document.Parse(json.c_str());
  auto overview_object = OverviewObject{};
  for (auto const& country_data : document["data"].GetArray())
  {
    OverviewObject::country_t country;
    country.name = country_data["name"].GetString();
    country.code = country_data["code"].GetString();
    overview_object.countries.emplace_back(country);
  }
  return overview_object;
}

} // namespace coronan