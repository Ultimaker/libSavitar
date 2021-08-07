cmake_minimum_required(VERSION 3.13)
project(savitar)

option(BUILD_PYTHON "Build " ON)
option(BUILD_STATIC "Build as a static library" OFF)
option(BUILD_TESTS "Building the test-suite" OFF)

include(cmake/StandardProjectSettings.cmake)
include(CMakePackageConfigHelpers)
include(GenerateExportHeader)

set(SAVITAR_VERSION 0.1.2)
set(SAVITAR_SOVERSION 0)

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

if(BUILD_SHARED_LIBS)
    add_library(Savitar SHARED ${savitar_SRCS})
else()
    add_library(Savitar STATIC ${savitar_SRCS})
endif()

use_threads(Savitar)

set_project_standards(Savitar)
set_project_warnings(Savitar)
enable_sanitizers(Savitar)

set(USE_SHIPPED_PUGIXML ON CACHE BOOL "Use the shipped pugixml library")
if(USE_SHIPPED_PUGIXML)
    message(STATUS "Using shipped pugixml library")
    add_subdirectory(pugixml)
else()
    find_package(pugixml 1.8 REQUIRED)
endif()

target_link_libraries(Savitar PRIVATE pugixml::pugixml)

generate_export_header(Savitar EXPORT_FILE_NAME src/SavitarExport.h)
target_include_directories(Savitar
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/src> # Include SavitarExport.h
        )

if(BUILD_PYTHON)
    list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

    find_package(SIP REQUIRED)

    set(SIP_EXTRA_FILES_DEPEND python/Types.sip python/MeshData.sip python/SceneNode.sip python/Scene.sip)
    set(SIP_EXTRA_OPTIONS -y Savitar.pyi -o -g -n PyQt5.sip)

    add_sip_python_module(Savitar python/ThreeMFParser.sip Savitar)

    set_project_standards(python_module_Savitar)
    set_project_warnings(python_module_Savitar)
    use_python(python_module_Savitar Interpreter Development)

    target_link_libraries(python_module_Savitar PRIVATE SIP::SIP)
    target_include_directories(python_module_Savitar
            PRIVATE
                $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
                $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/python>
                $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/src> # Include SavitarExport.h
            )
endif()

if(${CMAKE_BUILD_TYPE})
    if(${CMAKE_BUILD_TYPE} STREQUAL "Debug" OR ${CMAKE_BUILD_TYPE} STREQUAL "RelWithDebInfo")
        add_definitions(-DSAVITAR_DEBUG)
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

install(EXPORT Savitar-targets DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Savitar)

configure_package_config_file(SavitarConfig.cmake.in ${CMAKE_BINARY_DIR}/SavitarConfig.cmake INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Savitar)
write_basic_package_version_file(${CMAKE_BINARY_DIR}/SavitarConfigVersion.cmake VERSION ${SAVITAR_VERSION} COMPATIBILITY SameMajorVersion)

# Compiling the test environment.
if (BUILD_TESTS)
    message(STATUS "Building with tests...")
    find_package(GTest CONFIG REQUIRED)

    # List of tests. For each test there must be a file tests/${NAME}.cpp.
    set(savitar_TEST
            ThreeMFParserTest
            MeshDataTest
            NamespaceTest
            )

    enable_testing()

    #To make sure that the tests are built before running them, add the building of these tests as an additional test.
    add_custom_target(build_all_tests)
    add_test(BuildTests "${CMAKE_COMMAND}" --build ${CMAKE_CURRENT_BINARY_DIR} --target build_all_tests)

    foreach (test ${savitar_TEST})
        add_executable(${test} tests/main.cpp tests/${test}.cpp)
        set_project_standards(${test})
        set_project_warnings(${test})
        target_link_libraries(${test} PRIVATE Savitar GTest::gtest GTest::gmock pugixml::pugixml)
        add_test(${test} ${test})
        add_dependencies(build_all_tests ${test}) #Make sure that this gets built as part of the build_all_tests target.
    endforeach()
endif()

install(FILES
    ${CMAKE_BINARY_DIR}/SavitarConfig.cmake
    ${CMAKE_BINARY_DIR}/SavitarConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Savitar
)
