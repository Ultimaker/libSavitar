cmake_policy(SET CMP0091 NEW)
project(PackageTest)
cmake_minimum_required(VERSION 3.20)

find_package(savitar REQUIRED)

add_executable(test test.cpp)

target_link_libraries(test
        PUBLIC
            savitar::savitar
        )