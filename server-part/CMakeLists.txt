cmake_minimum_required(VERSION 3.12)
project(code-analytics)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(contribs/clickhouse-cpp)

add_executable(${PROJECT_NAME} "main.cpp")

target_sources(code-analytics PRIVATE main.cpp ./tools/TableManager.h ./tools/TableManager.cpp ./tools/MinHashMaker.h ./tools/MinHashMaker.cpp)

# Uncomment to use sha256
#find_package(OpenSSL REQUIRED)
#include_directories(${OPENSSL_INCLUDE_DIR})
#target_link_libraries(YourExecutableName ${OPENSSL_LIBRARIES})

target_include_directories(code-analytics PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_include_directories(${PROJECT_NAME} PRIVATE contribs/clickhouse-cpp/ contribs/clickhouse-cpp/contrib/absl)

target_link_libraries(${PROJECT_NAME} PRIVATE clickhouse-cpp-lib)