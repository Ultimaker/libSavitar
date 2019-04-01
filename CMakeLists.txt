project(savitar)
cmake_minimum_required(VERSION 3.6)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

option(BUILD_PYTHON "Build " ON)
option(BUILD_STATIC "Build as a static library" OFF)
option(BUILD_TESTS "Building the test-suite" OFF)

if(BUILD_TESTS)
    message(STATUS "Building with tests...")
    find_package(GTest REQUIRED)
    find_package(Threads QUIET)
endif()

add_subdirectory(pugixml)

if(BUILD_PYTHON)
    set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

    # Use FindPython 3 because if we use 3.4 for instance, it will prefer 3.4, even if there is another
    # Python (say 3.5) in CMAKE_PREFIX_PATH which also meets the requirement. We prefer the Python in
    # CMAKE_PREFIX_PATH if set for our building process, so we specify Python 3 here.
    find_package(PythonInterp 3 REQUIRED)
    find_package(PythonLibs 3 REQUIRED)

    find_package(SIP REQUIRED)
    if(NOT DEFINED LIB_SUFFIX)
        set(LIB_SUFFIX "")
    endif()
    if(EXISTS /etc/debian_version)
        # Running on a debian-based system, which requires special handling for python modules.
        set(PYTHON_SITE_PACKAGES_DIR lib${LIB_SUFFIX}/python3/dist-packages CACHE STRING "Directory to install Python bindings to")
    else()
        set(PYTHON_SITE_PACKAGES_DIR lib${LIB_SUFFIX}/python${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}/site-packages CACHE STRING "Directory to install Python bindings to")
    endif()
    include_directories(python/ src/ ${SIP_INCLUDE_DIR} ${PYTHON_INCLUDE_DIR})
endif()

set(CMAKE_CXX_STANDARD 11)

if(APPLE AND CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif()

set(savitar_SRCS
    src/ThreeMFParser.cpp
    src/SceneNode.cpp
    src/Scene.cpp
    src/MeshData.cpp
    src/Vertex.cpp
    src/Face.cpp
)

set(savitar_HDRS
    src/ThreeMFParser.h
    src/Types.h
    src/SceneNode.h
    src/Scene.h
    src/MeshData.h
    src/Vertex.h
    src/Face.h
    src/SavitarExport.h
)

set(SAVITAR_VERSION 0.1.1)
set(SAVITAR_SOVERSION 0)

set(CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_FULL_LIBDIR})

if(BUILD_STATIC)
    add_library(Savitar STATIC ${savitar_SRCS})
else()
    add_library(Savitar SHARED ${savitar_SRCS})
endif()

set(Savitar_LINK_LIBRARIES pugixml)
if(CMAKE_USE_PTHREADS_INIT)
    list(APPEND Savitar_LINK_LIBRARIES pthread)
endif()
target_link_libraries(Savitar PUBLIC ${Savitar_LINK_LIBRARIES})

if(NOT WIN32 OR CMAKE_COMPILER_IS_GNUCXX)
    set_target_properties(Savitar PROPERTIES COMPILE_FLAGS -fPIC)
endif()

if(BUILD_PYTHON)
    set(SIP_EXTRA_FILES_DEPEND python/Types.sip python/MeshData.sip python/SceneNode.sip python/Scene.sip)
    #set(SIP_EXTRA_SOURCE_FILES python/Types.cpp)
    set(SIP_EXTRA_OPTIONS -g) # -g means always release the GIL before calling C++ methods.
    add_sip_python_module(Savitar python/ThreeMFParser.sip Savitar)
endif()

target_include_directories(Savitar PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

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
