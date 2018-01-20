#define CATCH_CONFIG_MAIN
#include "catch.h"

#include "serviceparser.h"

namespace test {

std::string readTextFile(const std::string& path)
{
    std::ifstream file(path.c_str());
    std::stringstream buffer;
    buffer << file.rdbuf();
    
    return buffer.str();
}

TEST_CASE( "ParseServiceXml", "[parser]" ) {
    auto serviceXml = readTextFile(TEST_DATA_DIR "/servicedesc.xml");
    auto serviceDesc = wmts::parseServiceDescription(serviceXml);
    
    REQUIRE(serviceDesc.name == "AGIV WMTS service");
    REQUIRE(serviceDesc.version == "1.0.0");
    
    auto l = serviceDesc.layers.front();
    REQUIRE(l.bbox.lowerCorner.longitude == Approx(2.53));
    REQUIRE(l.bbox.lowerCorner.latitude == Approx(50.685));
    
    REQUIRE(l.bbox.upperCorner.longitude == Approx(5.92));
    REQUIRE(l.bbox.upperCorner.latitude == Approx(51.52));
    
    REQUIRE(l.tileMatrixSets.size() == 3);
    auto ms = l.tileMatrixSets.front();
    REQUIRE(ms.limits.size() == 22);
    REQUIRE(ms.minZoomLevel == 0);
    REQUIRE(ms.maxZoomLevel == 21);
    
    
}

}
