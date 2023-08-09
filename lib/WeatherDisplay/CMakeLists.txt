add_library(WeatherDisplay WeatherDisplay.cpp)

target_link_libraries(WeatherDisplay PRIVATE cpr::cpr nlohmann_json::nlohmann_json)