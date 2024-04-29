cmake_minimum_required(VERSION 3.20)

set(PostgreSQL_ADDITIONAL_VERSIONS "16")

project(ClientsDB)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../libpqxx-7.9.0" libpqxx-build)

add_executable(ClientsDB main.cpp)

target_compile_features(ClientsDB PRIVATE cxx_std_17)

target_link_libraries(ClientsDB pqxx)