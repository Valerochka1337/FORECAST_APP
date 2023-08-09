#pragma once
#include "cpr/cpr.h"
#include "./WeatherParser/WeatherParser.h"
#include "ostream"

namespace WeatherDisplay {
  std::ostream& getCityWeatherStream(std::ostream& stream, const WeatherData::CityWeatherData& city_weather, size_t n_days);
  std::ostream& getDayWeatherStream(std::ostream& stream,  const WeatherData::DayWeatherData& day_weather);
};
