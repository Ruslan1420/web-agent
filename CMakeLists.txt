cmake_minimum_required(VERSION 3.15)
project(WebAgent)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Найти библиотеки
find_package(CURL REQUIRED)
find_package(cpr REQUIRED)
find_package(nlohmann_json REQUIRED)

# Создать исполняемый файл
add_executable(web_agent src/main.cpp)

# Связать библиотеки
target_link_libraries(web_agent PRIVATE cpr::cpr nlohmann_json::nlohmann_json)
