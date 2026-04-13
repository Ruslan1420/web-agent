cmake_minimum_required(VERSION 3.15)
project(WebAgent)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(CURL REQUIRED)

set(CPR_INCLUDE_DIR "/usr/local/include")
set(CPR_LIBRARY "/usr/local/lib/libcpr.so")
set(JSON_INCLUDE_DIR "/usr/include/nlohmann")

add_executable(web_agent src/main.cpp)

target_include_directories(web_agent PRIVATE 
    ${CPR_INCLUDE_DIR}
    ${JSON_INCLUDE_DIR}
)

target_link_libraries(web_agent PRIVATE 
    ${CPR_LIBRARY} 
    ${CURL_LIBRARIES}
)