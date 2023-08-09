add_library(WeatherParser WeatherParser.cpp)

target_link_libraries(WeatherParser PRIVATE cpr::cpr nlohmann_json::nlohmann_json)