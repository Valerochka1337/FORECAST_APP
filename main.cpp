#include <iostream>
#include "conio.h"
#include "lib/WeatherDisplay/WeatherDisplay.h"

int main() {
  WeatherParser::Parser parser("/Users/valerijarcev/CLionProjects/labwork-10-Valerochka1337/config.json");

  const size_t max_city_ind = parser.getConfig().cities.size() - 1;
  size_t cur_city_ind = 0;
  const size_t max_n_days = WeatherParser::kMaxNumDaysWeather;
  const size_t default_n_days = parser.getConfig().numDays;
  size_t cur_n_days = default_n_days;
  const size_t update_freq = parser.getConfig().updateFreq;

  WeatherDisplay::getCityWeatherStream(std::cout, parser.getWeather()[cur_city_ind], cur_n_days);
  size_t start_clock = clock();
  size_t end_clock = start_clock + update_freq;

  int key;
  while (true) {
    start_clock = clock();
    if (start_clock >= end_clock) {
      parser.updateWeather();
      system("clear");
      WeatherDisplay::getCityWeatherStream(std::cout, parser.getWeather()[cur_city_ind], cur_n_days);
      start_clock = clock();
      end_clock = start_clock + update_freq;
    }
    if (read_key(key)) {  // Check if a key is pressed
      if (key == 27) { // esc
        break;
      } else if (key == 43 || key == 45 || key == 61) { // + and -
        if (key == 43 || key == 61) {
          if (cur_n_days < max_n_days) {
            cur_n_days++;
            WeatherDisplay::getDayWeatherStream(std::cout,
                                                parser.getWeather()[cur_city_ind].daysWeather[cur_n_days-1]);
          }
        } else {
          if (cur_n_days > 0) {
            cur_n_days--;
            system("clear");
            WeatherDisplay::getCityWeatherStream(std::cout,
                                                 parser.getWeather()[cur_city_ind],
                                                 cur_n_days);
          }
        }
      } else if (key == 110 || key == 112) { // n and p
        if (key == 110) {
          if (cur_city_ind > 0) {
            cur_city_ind--;
            system("clear");
            WeatherDisplay::getCityWeatherStream(std::cout,
                                                 parser.getWeather()[cur_city_ind],
                                                 cur_n_days);
          }
        } else {
          if (cur_city_ind < max_city_ind) {
            cur_city_ind++;
            system("clear");
            WeatherDisplay::getCityWeatherStream(std::cout,
                                                 parser.getWeather()[cur_city_ind],
                                                 cur_n_days);
          }
        }
      }
    }
  }
  return 0;
}
