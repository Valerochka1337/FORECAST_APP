#include "WeatherDisplay.h"
#include "WeatherAssets.h"

std::string correctLength(const std::string& s, size_t n) {
  std::string new_s;
  if (s.length() <= n) {
    new_s = s;
    for (int i = 0; i < n - s.length(); ++i) {
      new_s += ' ';
    }
  } else {
    for (int i = 0; i < n - 3; ++i) {
      new_s += s[i];
    }
    new_s += "...";
  }
  return new_s;
}

std::vector<std::string> getDayBlock(const WeatherData::WeatherDataBlock& weather_block, bool is_cur = false) {
  std::vector<std::string> block = WeatherAsset::getWeatherImage(weather_block.weatherCode);
  int free_space = WeatherAsset::kBlockWidth - WeatherAsset::kImageWidth;

  // 1) Weather description
  block[0] += correctLength(WeatherAsset::weather_codes.find(weather_block.weatherCode)->second, free_space);

  // 2) temperature
  std::string tmp;
  if (weather_block.temperature >= 0) tmp += '+';
  tmp += std::to_string(static_cast<int>(weather_block.temperature)) + " °C";
  block[1] += correctLength(tmp, free_space) + ' ';

  // 3) wind direction + speed
  tmp.clear();
  tmp += WeatherAsset::getWindDirection(weather_block.windDirection) + ' ';
  tmp += std::to_string(static_cast<int> (weather_block.windSpeed)) + " km/h";
  block[2] += correctLength(tmp, free_space) + "  ";

  // 4) surface pressure (hPa)
  tmp.clear();
  if (!is_cur) tmp += std::to_string(static_cast<int>(weather_block.surfacePressure)) + " hPa";
  block[3] += correctLength(tmp, free_space);

  // 5) precipitation + percentage
  tmp.clear();
  if (!is_cur) tmp += std::to_string(static_cast<int> (weather_block.precipitation * 1000)) + " nm";
  if (!is_cur) tmp += " (" + std::to_string(weather_block.precipitationProbability) + "%)";
  block[4] += correctLength(tmp, free_space);

  return block;
}

std::vector<std::string> getDayWeatherTable(const WeatherData::DayWeatherData& day_weather) {
  const int table_width = WeatherAsset::kBlockWidth * WeatherParser::kNumDayBlocks;
  std::vector<std::string> table(WeatherAsset::kBlockHeight + 5);

  // date
  for (int i = 0; i < WeatherAsset::kBlockWidth * WeatherParser::kNumDayBlocks / 2 - 5; ++i) {
    table[0] += ' ';
  }
  // top border
  table[0] += "Date:" + day_weather.date;
  for (int i = 0; i < table_width + WeatherParser::kNumDayBlocks + 1; ++i) {
    table[1] += '_';
  }

  table[2] += '|';
  size_t it = 0;
  for (size_t j = 1; j < table_width + WeatherParser::kNumDayBlocks + 1; ++j) {
    if (j % WeatherAsset::kBlockWidth == WeatherAsset::kBlockWidth/2) {
      table[2] += WeatherAsset::day_parts[it] + "  ";
      j += WeatherAsset::day_parts[it].length();
      it += 1;
    } else if (j % WeatherAsset::kBlockWidth == 0) {
      table[2] += '|';
    } else {
      table[2] += ' ';
    }
  }
  for (int j = 1; j < table_width + WeatherParser::kNumDayBlocks; ++j) {
    table[3] += '_';
  }

  for (int i = 0; i < WeatherParser::kNumDayBlocks; ++i) {
    std::vector<std::string> block = getDayBlock(day_weather.weatherBlocks[i]);

    for (int j = 4; j < WeatherAsset::kBlockHeight + 4; ++j) {
      // block
      table[j] += '|';
      table[j] += block[j - 4];
    }
  }
  for (int i = 4; i < WeatherAsset::kBlockHeight + 4; ++i) {
    table[i] += '|';
  }
  for (int i = 0; i < table_width + WeatherParser::kNumDayBlocks + 1; ++i) {
    table[WeatherAsset::kBlockHeight + 4] += '_';
  }
  return table;
}

std::ostream& WeatherDisplay::getCityWeatherStream(std::ostream& stream,
                                                   const WeatherData::CityWeatherData& city_weather,
                                                   size_t n_days) {
  stream << "Weather Forecast for: " << city_weather.cityName << '\n';
  stream << "Current weather:\n";
  std::vector<std::string> cur_weather = getDayBlock(city_weather.currentWeather, true);
  for (int i = 0; i < WeatherAsset::kBlockHeight; ++i) {
    stream << cur_weather[i] << '\n';
  }
  for (int i = 0; i < n_days && i < city_weather.daysWeather.size(); ++i) {
    getDayWeatherStream(stream, city_weather.daysWeather[i]);
  }
  return stream;
}
std::ostream& WeatherDisplay::getDayWeatherStream(std::ostream& stream,
                                                  const WeatherData::DayWeatherData& day_weather) {
  std::vector<std::string> day_weather_data = getDayWeatherTable(day_weather);
  for (auto& line : day_weather_data) {
    stream << line << "\n";
  }
  stream << '\n';
  return stream;
}