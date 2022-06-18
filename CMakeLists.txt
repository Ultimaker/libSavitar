# For MSVC flags, will be ignored on non-Windows OS's
cmake_policy(SET CMP0091 NEW)

project(savitar)
cmake_minimum_required(VERSION 3.20)
include(cmake/StandardProjectSettings.cmake)

option(BUILD_PYTHON "Build " ON)
option(BUILD_TESTS "Building the test-suite" OFF)

if(BUILD_TESTS)
    message(STATUS "Building with tests...")
    find_package(GTest REQUIRED)
endif()

find_package(pugixml REQUIRED)

set(savitar_SRCS
    src/Namespace.cpp
    src/ThreeMFParser.cpp
    src/SceneNode.cpp
    src/Scene.cpp
    src/MeshData.cpp
    src/Vertex.cpp
    src/Face.cpp
)

if(BUILD_SHARED_LIBS)
    add_library(Savitar SHARED ${savitar_SRCS})
    if(WIN32)
        set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
        set_target_properties(Savitar PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
    endif()
else()
    add_library(Savitar STATIC ${savitar_SRCS})
endif()
set_project_warnings(Savitar)

target_link_libraries(Savitar PUBLIC pugixml::pugixml)

target_include_directories(Savitar
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
        PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        )

if(CMAKE_BUILD_TYPE)
    if(${CMAKE_BUILD_TYPE} STREQUAL "Debug" OR ${CMAKE_BUILD_TYPE} STREQUAL "RelWithDebInfo")
        target_compile_definitions(Savitar PUBLIC -DSAVITAR_DEBUG)
    endif()
endif()

if(BUILD_PYTHON)
    list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
    find_package(cpython ${Python_VERSION} QUIET COMPONENTS Interpreter Development)

    find_package(SIP REQUIRED 6.5.0)

    add_library(pySavitar INTERFACE)

    target_include_directories(pySavitar
            INTERFACE
                $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
                $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/python/>
            )

    target_link_libraries(pySavitar INTERFACE Savitar pugixml::pugixml cpython::cpython)

    add_sip_module(pySavitar)
    if (DEFINED Python_SITELIB_LOCAL)
        install_sip_module(pySavitar ${Python_SITELIB_LOCAL})
    else()
        install_sip_module(pySavitar)
    endif ()
endif()

# List of tests. For each test there must be a file tests/${NAME}.cpp.
set(savitar_TEST
    ThreeMFParserTest
    MeshDataTest
    NamespaceTest
)

# Compiling the test environment.
if (BUILD_TESTS)
    include_directories(${GTEST_INCLUDE_DIR})

    enable_testing()

    #To make sure that the tests are built before running them, add the building of these tests as an additional test.
    add_custom_target(build_all_tests)
    add_test(BuildTests "${CMAKE_COMMAND}" --build ${CMAKE_CURRENT_BINARY_DIR} --target build_all_tests)

    foreach (test ${savitar_TEST})
        add_executable(${test} tests/main.cpp tests/${test}.cpp)
        target_link_libraries(${test} Savitar ${GTEST_BOTH_LIBRARIES})
        add_test(${test} ${test})
        add_dependencies(build_all_tests ${test}) #Make sure that this gets built as part of the build_all_tests target.
    endforeach()
endif()
