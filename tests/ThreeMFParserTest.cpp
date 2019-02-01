#include "ThreeMFParserTest.h"

#include "../src/Scene.h"
#include "../src/SceneNode.h"
#include "../src/ThreeMFParser.h"

#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <string>

namespace Savitar
{
    CPPUNIT_TEST_SUITE_REGISTRATION(ThreeMFParserTest);

    void ThreeMFParserTest::setUp()
    {
        xml_string = "";
        std::ifstream test_model_file("../tests/test_model.xml");
        if (test_model_file.is_open())
        {
            xml_string = std::string(std::istreambuf_iterator<char>{test_model_file}, {});
        }

        parser = new ThreeMFParser();
    }

    void ThreeMFParserTest::tearDown()
    {
        delete parser;
    }

    void ThreeMFParserTest::parseTest()
    {
        CPPUNIT_ASSERT(! xml_string.empty());

        Scene scene;
        CPPUNIT_ASSERT_NO_THROW(scene = parser->parse(xml_string));

        std::vector<SceneNode*> nodes = scene.getSceneNodes();
        CPPUNIT_ASSERT(! nodes.empty());
        CPPUNIT_ASSERT_EQUAL(nodes.size(), 3UL);

        for (SceneNode* node : nodes)
        {
            MeshData& data = node->getMeshData();

            const auto& verts = data.getVertices();
            CPPUNIT_ASSERT_EQUAL(verts.size(), 36UL);

            const auto& tris = data.getFacesAsBytes();
            CPPUNIT_ASSERT(! tris.empty());
        }
        // NOTE: To/from for content of vertices/triangles is tested in MeshDataTest.

        std::map<std::string, std::string> settings;

        settings = nodes[0]->getSettings();
        CPPUNIT_ASSERT(settings.find("extruder_nr") != settings.end());
        CPPUNIT_ASSERT_EQUAL(settings["extruder_nr"].compare("0"), 0);
        CPPUNIT_ASSERT(settings.find("bottom_layers") != settings.end());
        CPPUNIT_ASSERT_EQUAL(settings["bottom_layers"].compare("20"), 0);
        CPPUNIT_ASSERT(settings.find("infill_pattern") == settings.end());

        settings = nodes[1]->getSettings();
        CPPUNIT_ASSERT(settings.empty());

        settings = nodes[2]->getSettings();
        CPPUNIT_ASSERT(settings.find("extruder_nr") != settings.end());
        CPPUNIT_ASSERT_EQUAL(settings["extruder_nr"].compare("1"), 0);
        CPPUNIT_ASSERT(settings.find("bottom_layers") == settings.end());
        CPPUNIT_ASSERT(settings.find("infill_pattern") != settings.end());
        CPPUNIT_ASSERT_EQUAL(settings["infill_pattern"].compare("concentric"), 0);

        CPPUNIT_ASSERT_EQUAL(nodes[1]->getId().compare("2"), 0);
        CPPUNIT_ASSERT(! nodes[1]->getTransformation().empty());
    }

    void ThreeMFParserTest::sceneToStringTest()
    {
        CPPUNIT_ASSERT(! xml_string.empty());
        Scene scene = parser->parse(xml_string);

        const std::string scene_string = parser->sceneToString(scene);
        CPPUNIT_ASSERT(! scene_string.empty());
    }

} // namespace Savitar
