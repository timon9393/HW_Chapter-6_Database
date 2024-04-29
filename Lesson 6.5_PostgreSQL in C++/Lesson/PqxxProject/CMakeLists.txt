cmake_minimum_required(VERSION 3.20)

set(PostgreSQL_ADDITIONAL_VERSIONS "16")

project(PxxProject)

add_subdirectory("F:/Study/Netology/Dev Cpp/Chapter 6_Database/Lesson 6.5_PostgreSQL in C++/Lesson/libpqxx-7.9.0" libpqxx-build)

add_executable(PqxxProject main.cpp)

target_compile_features(PqxxProject PRIVATE cxx_std_17)

target_link_libraries(PqxxProject pqxx)