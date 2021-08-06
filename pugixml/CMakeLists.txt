cmake_minimum_required(VERSION 3.13)
project(pugixml)
include(${CMAKE_SOURCE_DIR}/cmake/StandardProjectSettings.cmake)

option(BUILD_SHARED_LIBS "Build shared instead of static library" OFF)
option(BUILD_TESTS "Build tests" OFF)
option(BUILD_PKGCONFIG "Build in PKGCONFIG mode" OFF)

set(BUILD_DEFINES "" CACHE STRING "Build defines")

if (MSVC)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ${BUILD_SHARED_LIBS})
endif()

# Pre-defines standard install locations on *nix systems.
include(GNUInstallDirs)
mark_as_advanced(CLEAR CMAKE_INSTALL_LIBDIR CMAKE_INSTALL_INCLUDEDIR)

set(SOURCES ${HEADERS} src/pugixml.cpp)

if(DEFINED BUILD_DEFINES)
    foreach(DEFINE ${BUILD_DEFINES})
        add_definitions("-D" ${DEFINE})
    endforeach()
endif()

if(BUILD_SHARED_LIBS)
    add_library(pugixml SHARED ${SOURCES})
else()
    add_library(pugixml STATIC ${SOURCES})
endif()

use_threads(pugixml)

target_include_directories(pugixml
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
        )

set_project_standards(pugixml)
set_project_warnings(pugixml)

# Enable C++11 long long for compilers that are capable of it
if(NOT ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} STRLESS 3.1 AND ";${CMAKE_CXX_COMPILE_FEATURES};" MATCHES ";cxx_long_long_type;")
    target_compile_features(pugixml PUBLIC cxx_long_long_type)
endif()

set_target_properties(pugixml PROPERTIES VERSION 1.8 SOVERSION 1)
get_target_property(PUGIXML_VERSION_STRING pugixml VERSION)

if(BUILD_PKGCONFIG)
    # Install library into its own directory under LIBDIR
    set(INSTALL_SUFFIX /pugixml-${PUGIXML_VERSION_STRING})
endif()

install(TARGETS pugixml EXPORT pugixml-config
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}${INSTALL_SUFFIX}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}${INSTALL_SUFFIX}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES ${HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}${INSTALL_SUFFIX})
install(EXPORT pugixml-config DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/pugixml)

if(BUILD_PKGCONFIG)
    configure_file(scripts/pugixml.pc.in ${PROJECT_BINARY_DIR}/pugixml.pc @ONLY)
    install(FILES ${PROJECT_BINARY_DIR}/pugixml.pc DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/pkgconfig)
endif()

add_library(pugixml::pugixml ALIAS pugixml)
