#include "iostream"
#include "vector"

namespace WeatherAsset {
const int kBlockHeight = 5;
const int kImageWidth = 16;
const int kBlockWidth = 18 + kImageWidth;
const int kImageHeight = 5;

const std::vector<std::string> cloudy = {"                ",
                                         "      .--.      ",
                                         "   .-(    ).    ",
                                         "  (___.__)__)   ",
                                         "                "};

const std::vector<std::string> sunny = {"     \\__|__/    ",
                                        "     /     \\    ",
                                        "   -|       |-  ",
                                        "     \\_____/    ",
                                        "     /  |  \\    "};

const std::vector<std::string> foggy = {"                ",
                                        "   -_-_-_-_-_   ",
                                        "   _-_-_-_-_-   ",
                                        "   -_-_-_-_-_   ",
                                        "                "};

const std::vector<std::string> snowy = {"       .-.      ",
                                        "      (   ).    ",
                                        "     (___(__)   ",
                                        "     * * * *    ",
                                        "    * * * *     "};

const std::vector<std::string> rainy = {"       .-.      ",
                                        "      (   ).    ",
                                        "     (___(__)   ",
                                        "     / , / ,    ",
                                        "    , / , /     "};

const std::vector<std::string> overcast = {"                ",
                                           "      .--.      ",
                                           "   .-(    ).    ",
                                           "  (___.__)__)   ",
                                           "    /  /  /     "};

const std::vector<std::string> snowy_rainy = {"       .-.      ",
                                              "      (   ).    ",
                                              "     (___(__)   ",
                                              "     / * / *    ",
                                              "    * / * /     "};

const std::unordered_map<int, std::string> weather_codes = {
    {0, "Clear sky"},//s
    {1, "Mainly Clear"},//s
    {2, "Partly Cloudy"},//c
    {3, "Overcast"},//o
    {45, "Fog"},//f
    {48, "Fog"},//f
    {51, "Drizzle"},//r
    {53, "Drizzle"},//r
    {55, "Drizzle"},//r
    {56, "Freezing Drizzle"},//rs
    {57, "Freezing Drizzle"},//rs
    {61, "Slight Rain"},//r
    {63, "Moderate Rain"},//r
    {65, "Heavy Rain"},//r
    {66, "Light Freezing Rain"},//rs
    {67, "Heavy Freezing Rain"},//rs
    {71, "Slight Snowfall"},//s
    {73, "Moderate Snowfall"},//s
    {75, "Heavy Snowfall"},//s
    {77, "Snow grains"},//s
    {80, "Slight Rain shower"},//r
    {81, "Moderate Rain shower"},//r
    {82, "Violent Rain shower"},//r
    {85, "Slight Snow shower"},//s
    {86, "Heavy Snow shower"},//s
    {95, "Thunderstorm"},//r
    {96, "Thunderstorm with slight hail"},//r
    {99, "Thunderstorm with heavy hail"}//r
};
const std::vector<std::string>& getWeatherImage(int code) {
  if (code <= 1) {
    return sunny;
  } else if (code <= 2) {
    return cloudy;
  } else if (code <= 3) {
    return overcast;
  } else if (code <= 48) {
    return foggy;
  } else if (code <= 55) {
    return rainy;
  } else if (code <= 57) {
    return snowy_rainy;
  } else if (code <= 65) {
    return rainy;
  } else if (code <= 67) {
    return snowy_rainy;
  } else if (code <= 77) {
    return snowy;
  } else if (code <= 82) {
    return rainy;
  } else if (code <= 86) {
    return snowy;
  }
    return rainy;
}
std::string getWindDirection(int degree) {
  if (337.5 <= degree or degree < 22.5) return "↑ N";
  else if (22.5 <= degree and degree < 67.5) return "↗ NE";
  else if (67.5 <= degree and degree < 112.5) return "→ E";
  else if (112.5 <= degree and degree < 157.5) return "↘ SE";
  else if (157.5 <= degree and degree < 202.5) return "↓ S";
  else if (202.5 <= degree and degree < 247.5) return "↙ SW";
  else if (247.5 <= degree and degree < 292.5) return "← W";
  else if (292.5 <= degree and degree < 337.5) return "↖ NW";
  return {};
}

const std::vector<std::string> day_parts = {"Morning", "Day", "Evening", "Night"};

}