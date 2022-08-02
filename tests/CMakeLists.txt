find_package(GTest REQUIRED)
include(GoogleTest)

set(savitar_TEST
        ThreeMFParserTest
        MeshDataTest
        NamespaceTest
        )

foreach (test ${savitar_TEST})
    add_executable(${test} main.cpp ${test}.cpp)
    target_link_libraries(${test} Savitar GTest::gtest GTest::gmock)
    add_test(${test} ${test})
endforeach()