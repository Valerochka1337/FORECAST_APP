#pragma once
#include "iostream"
#include "cpr/cpr.h"
#include "nlohmann/json.hpp"
#include "WeatherData/WeatherData.h"

namespace WeatherParser {
const std::string kNinjasApiKey = "nQQJ53uqfurbKRyclY1xyg==Hj1GwDPvJI7FynhN";
const int kMaxNumDaysWeather = 16;
const int kFreqHour = 6;
const int kDayHours = 24;
const int kNumDayBlocks = kDayHours/kFreqHour;

struct config {
  std::vector<std::string> cities;
  uint32_t numDays;
  uint32_t updateFreq;
};

class Parser {
 public:
  Parser() = default;
  explicit Parser(const std::string& config_path);
  ~Parser() = default;

  void updateWeather();
  void setConfig(const std::string& config_path);
  const config& getConfig();
  const std::vector<WeatherData::CityWeatherData>& getWeather();
 private:
  config config_;
  std::vector<WeatherData::CityWeatherData> citiesWeather_;
};

WeatherData::CityWeatherData getCityWeather(const std::string& city_name);
std::pair<double, double> parseCityCoordinates(const std::string& city_name);
config parseConfig(const std::string& config_path);
Parser parse(const std::string& config_path);
}