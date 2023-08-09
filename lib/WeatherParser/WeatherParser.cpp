#include "WeatherParser.h"

WeatherData::CityWeatherData WeatherParser::getCityWeather(const std::string& city_name) {
  const int kNumBlockHours = WeatherParser::kFreqHour;
  const int kNumDayHours = WeatherParser::kDayHours;
  const int kNumDayBlocks = WeatherParser::kNumDayBlocks;
  const int kStartHour = 5;

  WeatherData::CityWeatherData city_weather;
  city_weather.cityName = city_name;
  std::pair<double, double> coordinates = parseCityCoordinates(city_name);
  city_weather.latitude = coordinates.first;
  city_weather.longitude = coordinates.second;
  city_weather.daysWeather = std::vector<WeatherData::DayWeatherData>(WeatherParser::kMaxNumDaysWeather);
  cpr::Parameters params = {
      {"latitude", std::to_string(city_weather.latitude)},
      {"longitude", std::to_string(city_weather.longitude)},
      {"hourly", "temperature_2m,relativehumidity_2m,precipitation_probability,"
                 "precipitation,weathercode,surface_pressure,windspeed_10m,winddirection_10m"},
      {"current_weather", "true"},
      {"forecast_days", "16"}
  };
  cpr::Response w = cpr::Get(cpr::Url("https://api.open-meteo.com/v1/forecast"), params);
  nlohmann::json weather = nlohmann::json::parse(w.text);

  city_weather.currentWeather.time = weather["current_weather"]["time"];
  city_weather.currentWeather.windDirection = static_cast<int>(weather["current_weather"]["winddirection"]);
  city_weather.currentWeather.windSpeed = static_cast<double>(weather["current_weather"]["windspeed"]);
  city_weather.currentWeather.temperature = static_cast<double>(weather["current_weather"]["temperature"]);
  city_weather.currentWeather.weatherCode = static_cast<int>(weather["current_weather"]["weathercode"]);

  uint32_t hours_count = kStartHour;
  for (auto& day : city_weather.daysWeather) {
    uint32_t day_hours = hours_count;
    day.date = weather["hourly"]["time"][hours_count];
    day.date = day.date.substr(0, day.date.size() - 6);
    day.weatherBlocks = std::vector<WeatherData::WeatherDataBlock>(kNumDayBlocks);
    for (size_t day_part = 0; day_part < kNumDayBlocks; ++day_part, day_hours += kNumBlockHours) {

      day.weatherBlocks[day_part].time = weather["hourly"]["time"][day_hours];
      day.weatherBlocks[day_part].time = day.weatherBlocks[day_part].time.substr(11, 5);

      auto temp_val = weather["hourly"]["temperature_2m"][day_hours];
      day.weatherBlocks[day_part].temperature = temp_val.is_null() ? 0.0 : static_cast<double>(temp_val);

      auto precip_val = weather["hourly"]["precipitation"][day_hours];
      day.weatherBlocks[day_part].precipitation = precip_val.is_null() ? 0.0 : static_cast<double>(precip_val);

      auto surface_val = weather["hourly"]["surface_pressure"][day_hours];
      day.weatherBlocks[day_part].surfacePressure = surface_val.is_null() ? 0.0 : static_cast<double>(surface_val);

      auto wind_val = weather["hourly"]["windspeed_10m"][day_hours];
      day.weatherBlocks[day_part].windSpeed = wind_val.is_null() ? 0.0 : static_cast<double>(wind_val);

      auto wind_dir = weather["hourly"]["winddirection_10m"][day_hours];
      day.weatherBlocks[day_part].windDirection = wind_dir.is_null() ? 0 : static_cast<int> (wind_dir);

      auto weather_code = weather["hourly"]["weathercode"][day_hours];
      day.weatherBlocks[day_part].weatherCode = weather_code.is_null() ? 0 : static_cast<int> (weather_code);

      auto precip_prob = weather["hourly"]["precipitation_probability"][day_hours];
      day.weatherBlocks[day_part].precipitationProbability = precip_prob.is_null() ? 0 : static_cast<int> (precip_prob);

      day.weatherBlocks[day_part].timeOfTheDay = day_part;
    }
    hours_count += kNumDayHours;
  }
  return city_weather;
}

std::pair<double, double> WeatherParser::parseCityCoordinates(const std::string& city_name) {
  cpr::Response r = cpr::Get(cpr::Url{"https://api.api-ninjas.com/v1/city"},
                             cpr::Parameters{{"name", city_name}},
                             cpr::Header{{"X-Api-Key", WeatherParser::kNinjasApiKey}});
  nlohmann::json coordinates_json = nlohmann::json::parse(r.text);
  if (coordinates_json.empty()) {
    std::cout << "Typo in the name of city: " << city_name << '\n';
    return std::make_pair(0, 0);
  }
  std::pair<double, double> coordinates = {static_cast<double> (coordinates_json[0]["latitude"]),
                                           static_cast<double> (coordinates_json[0]["longitude"])};

  return coordinates;
}

WeatherParser::config WeatherParser::parseConfig(const std::string& config_path) {
  std::ifstream config_file(config_path);
  WeatherParser::config config;
  if (!config_file.is_open()) {
    throw std::invalid_argument("config file can not be opened");
  } else {
    nlohmann::json config_json = nlohmann::json::parse(config_file);
    config.numDays = config_json["days"];
    config.cities = std::vector<std::string>(config_json["cities"].size());
    uint32_t it = 0;
    for (auto& city : config_json["cities"]) {
      config.cities[it] = city;
      it++;
    }
    config.updateFreq = config_json["freq"];
    config_file.close();
  }

  return config;
}

WeatherParser::Parser::Parser(const std::string& config_path) {
  config_ = parseConfig(config_path);
  updateWeather();
}
void WeatherParser::Parser::updateWeather() {
  citiesWeather_ = std::vector<WeatherData::CityWeatherData>(config_.cities.size());
  for (int i = 0; i < citiesWeather_.size(); ++i) {
    citiesWeather_[i] = getCityWeather(config_.cities[i]);
  }
}

const WeatherParser::config& WeatherParser::Parser::getConfig() {
  return config_;
}
const std::vector<WeatherData::CityWeatherData>& WeatherParser::Parser::getWeather() {
  return citiesWeather_;
}
void WeatherParser::Parser::setConfig(const std::string& config_path) {
  config_ = parseConfig(config_path);
  updateWeather();
}
WeatherParser::Parser WeatherParser::parse(const std::string& config_path) {
  return WeatherParser::Parser(config_path);
}