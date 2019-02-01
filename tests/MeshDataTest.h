//Copyright (c) 2019 Ultimaker B.V.
//libSavitar is released under the terms of the AGPLv3 or higher.

#ifndef SCENE_NODE_TEST_H
#define SCENE_NODE_TEST_H

#include <functional> // function
#include <string>

#include "../pugixml/src/pugixml.hpp"

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace Savitar
{
class MeshData;

class MeshDataTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(MeshDataTest);
    CPPUNIT_TEST(toXmlNodeTest);
    CPPUNIT_TEST(fromXmlNodeTest);
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

    void toXmlNodeTest();
    void fromXmlNodeTest();

private:

    const uint8_t* vertex_bytes;
    const uint8_t* face_bytes;

    pugi::xml_node node;
    MeshData* mesh_data;
};



} // namespace Savitar

#endif // SCENE_NODE_TEST_H
