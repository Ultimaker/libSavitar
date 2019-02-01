//Copyright (c) 2019 Ultimaker B.V.
//libSavitar is released under the terms of the AGPLv3 or higher.

#ifndef THREEMF_PARSER_TEST_H
#define THREEMF_PARSER_TEST_H

#include <functional> // function
#include <string>

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace Savitar
{

class ThreeMFParser;

class ThreeMFParserTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(ThreeMFParserTest);
    CPPUNIT_TEST(parseTest);
    CPPUNIT_TEST(sceneToStringTest);
    CPPUNIT_TEST_SUITE_END();

public:
    /*!
     * \brief Sets up the test suite to prepare for testing.
     */
    void setUp();

    /*!
     * \brief Tears down the test suite when testing is done.
     */
    void tearDown();
    
    //These are the actual test cases. The name of the function sort of describes what it tests but I refuse to document all of these, sorry.

    void parseTest();
    void sceneToStringTest();

private:
    
    std::string xml_string;
    ThreeMFParser* parser;
};

} // namespace Savitar

#endif // THREEMF_PARSER_TEST_H
