cmake_minimum_required(VERSION 3.17)

project(
        labwork-10-Valerochka1337
        VERSION 1.0
        DESCRIPTION "C++ Year 2023 Lab10"
        LANGUAGES CXX
)


set(CMAKE_CXX_STANDARD 17)

include_directories(lib)
add_subdirectory(lib/WeatherParser)
add_subdirectory(lib/WeatherDisplay)
add_executable(labwork-10-Valerochka1337
        main.cpp conio.h lib/WeatherParser/WeatherParser.h lib/WeatherDisplay/WeatherDisplay.h)
include(FetchContent)
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git
        GIT_TAG 0817715923c9705e68994eb52ef9df3f6845beba) # The commit hash for 1.10.x. Replace with the latest from: https://github.com/libcpr/cpr/releases
FetchContent_MakeAvailable(cpr)
target_link_libraries(labwork-10-Valerochka1337 PRIVATE cpr::cpr)
target_link_libraries(labwork-10-Valerochka1337 PRIVATE WeatherParser)
target_link_libraries(labwork-10-Valerochka1337 PRIVATE WeatherDisplay)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)
target_link_libraries(labwork-10-Valerochka1337 PRIVATE nlohmann_json::nlohmann_json)
