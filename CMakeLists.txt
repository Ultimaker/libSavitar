project(savitar)
cmake_minimum_required(VERSION 3.20)
include(cmake/StandardProjectSettings.cmake)
AssureOutOfSourceBuilds()

include(CMakePackageConfigHelpers)
include(GenerateExportHeader)
generate_export_header(Savitar EXPORT_FILE_NAME src/SavitarExport.h)

option(BUILD_PYTHON "Build " ON)
option(BUILD_STATIC "Build as a static library" OFF)
option(BUILD_TESTS "Building the test-suite" OFF)

if(BUILD_TESTS)
    message(STATUS "Building with tests...")
    find_package(GTest REQUIRED)
endif()

add_subdirectory(pugixml)

set(savitar_SRCS
    src/Namespace.cpp
    src/ThreeMFParser.cpp
    src/SceneNode.cpp
    src/Scene.cpp
    src/MeshData.cpp
    src/Vertex.cpp
    src/Face.cpp
)

set(savitar_HDRS
    src/Namespace.h
    src/ThreeMFParser.h
    src/Types.h
    src/SceneNode.h
    src/Scene.h
    src/MeshData.h
    src/Vertex.h
    src/Face.h
    ${CMAKE_CURRENT_BINARY_DIR}/src/SavitarExport.h
)

set(SAVITAR_VERSION 0.1.2)
set(SAVITAR_SOVERSION 0)

if(BUILD_STATIC)
    add_library(Savitar STATIC ${savitar_SRCS})
else()
    add_library(Savitar SHARED ${savitar_SRCS})
endif()

set_project_standards(Savitar)
use_threads(Savitar)
set_rpath(TARGETS
            Savitar
          PATHS
            "$<$<PLATFORM_ID:Linux>:usr/bin>"
            "$<$<PLATFORM_ID:Linux>:usr/bin/lib>"
            "$<$<PLATFORM_ID:Darwin>:../lib>"
            "$<$<PLATFORM_ID:Darwin>:../Resources/lib>"
           RELATIVE)

target_link_libraries(Savitar PUBLIC pugixml)

if(BUILD_PYTHON)
    list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

    if(NOT DEFINED Python_VERSION)
        set(Python_VERSION
                3.10
                CACHE STRING "Python Version" FORCE)
        message(STATUS "Setting Python version to ${Python_VERSION}. Set Python_VERSION if you want to compile against an other version.")
    endif()
    if(APPLE)
        set(Python_FIND_FRAMEWORK NEVER)
    endif()
    find_package(cpython ${Python_VERSION} QUIET COMPONENTS Interpreter Development)
    if(NOT TARGET cpython::cpython)
        find_package(Python ${Python_VERSION} EXACT REQUIRED COMPONENTS Interpreter Development)
    else()
        add_library(Python::Python ALIAS cpython::cpython)
        set(Python_SITEARCH "${CMAKE_INSTALL_PREFIX}/lib/python3.10/site-packages")
        set(Python_EXECUTABLE ${cpython_PACKAGE_FOLDER_RELEASE}/bin/python3)
        set(ENV{PYTHONPATH} ${Python_SITEARCH})
    endif()
    message(STATUS "Linking and building ${project_name} against Python ${Python_VERSION}")

    find_package(SIP REQUIRED 6.5.0)

    add_library(pySavitar INTERFACE)
    set_project_standards(pySavitar)
    set_rpath(TARGETS
                pySavitar
              PATHS
                "$<$<PLATFORM_ID:Linux>:usr/bin>"
                "$<$<PLATFORM_ID:Linux>:usr/bin/lib>"
                "$<$<PLATFORM_ID:Darwin>:../lib>"
                "../../"
               RELATIVE)
    use_threads(pySavitar)

    target_include_directories(pySavitar
            INTERFACE
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
            $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/python/>
            )

    target_compile_features(pySavitar INTERFACE cxx_std_17)
    target_link_libraries(pySavitar INTERFACE Savitar Python::Python)

    add_sip_module(pySavitar)
    if(Python_SITELIB_LOCAL)
        install_sip_module(pySavitar ${Python_SITELIB_LOCAL})
    else()
        install_sip_module(pySavitar)
    endif()
endif()

target_include_directories(Savitar PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

if(${CMAKE_BUILD_TYPE})
    if(${CMAKE_BUILD_TYPE} STREQUAL "Debug" OR ${CMAKE_BUILD_TYPE} STREQUAL "RelWithDebInfo")
        target_compile_definitions(Savitar -DSAVITAR_DEBUG)
    endif()
endif()

set_target_properties(Savitar PROPERTIES
    FRAMEWORK FALSE
    VERSION ${SAVITAR_VERSION}
    SOVERSION ${SAVITAR_SOVERSION}
    PUBLIC_HEADER "${savitar_HDRS}"
    DEFINE_SYMBOL MAKE_SAVITAR_LIB
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN 1
)

install(TARGETS Savitar
    EXPORT Savitar-targets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/Savitar
)

install(EXPORT Savitar-targets
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Savitar
)

configure_package_config_file(SavitarConfig.cmake.in ${CMAKE_BINARY_DIR}/SavitarConfig.cmake INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Savitar)
write_basic_package_version_file(${CMAKE_BINARY_DIR}/SavitarConfigVersion.cmake VERSION ${SAVITAR_VERSION} COMPATIBILITY SameMajorVersion)

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

install(FILES
    ${CMAKE_BINARY_DIR}/SavitarConfig.cmake
    ${CMAKE_BINARY_DIR}/SavitarConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Savitar
)
