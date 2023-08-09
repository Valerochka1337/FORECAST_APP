namespace WeatherData {
// it is the weather in the morning/day/evening/night
struct WeatherDataBlock {
  std::string time{};
  uint8_t timeOfTheDay{};
  uint8_t weatherCode{};
  double temperature{};
  int32_t windDirection{};
  double windSpeed{};
  double precipitation{};
  uint8_t precipitationProbability{};
  double surfacePressure{};
};

struct DayWeatherData {
  std::string date{};
  std::vector<WeatherDataBlock> weatherBlocks{};
};

struct CityWeatherData {
  std::string cityName{};
  double latitude{};
  double longitude{};
  std::vector<DayWeatherData> daysWeather{};
  WeatherDataBlock currentWeather{};
};
}
