#define CATCH_CONFIG_MAIN
#include "catch.h"

#include "serviceparser.h"

namespace test
{

std::string readTextFile(const std::string& path)
{
    std::ifstream     file(path.c_str());
    std::stringstream buffer;
    buffer << file.rdbuf();

    return buffer.str();
}

TEST_CASE("ParseServiceXml", "[parser]")
{
    auto serviceXml  = readTextFile(TEST_DATA_DIR "/servicedesc.xml");
    auto serviceDesc = wmts::parseServiceDescription(serviceXml);

    REQUIRE(serviceDesc.name == "AGIV WMTS service");
    REQUIRE(serviceDesc.version == "1.0.0");
    REQUIRE(serviceDesc.layers.size() == 31);

    auto layer = serviceDesc.layers.front();
    REQUIRE(layer.bbox.lowerCorner.longitude == Approx(2.53));
    REQUIRE(layer.bbox.lowerCorner.latitude == Approx(50.685));
    REQUIRE(layer.bbox.upperCorner.longitude == Approx(5.92));
    REQUIRE(layer.bbox.upperCorner.latitude == Approx(51.52));

    REQUIRE(layer.defaultStyle == "GRB-Basiskaart");
    REQUIRE(layer.styles == std::vector<std::string>{"GRB-Basiskaart"});
    REQUIRE(layer.tileMatrixSetLinks.size() == 3);

    auto matrixSetLink = layer.tileMatrixSetLinks.front();
    REQUIRE(matrixSetLink.limits.size() == 22);
    REQUIRE(matrixSetLink.minZoomLevel == 0);
    REQUIRE(matrixSetLink.maxZoomLevel == 21);
    REQUIRE(matrixSetLink.limits.size() == 22);

    auto limit = matrixSetLink.limits.front();
    REQUIRE(limit.zoomLevel == 0);
    REQUIRE(limit.colRange.min == 0);
    REQUIRE(limit.colRange.max == 0);
    REQUIRE(limit.rowRange.min == 0);
    REQUIRE(limit.rowRange.max == 0);

    limit = matrixSetLink.getTileMatrixLimits(5);
    REQUIRE(limit.zoomLevel == 5);
    REQUIRE(limit.colRange.min == 11);
    REQUIRE(limit.colRange.max == 21);
    REQUIRE(limit.rowRange.min == 5);
    REQUIRE(limit.rowRange.max == 15);

    REQUIRE(serviceDesc.tilematrixSets.size() == 3);
    auto matrixSet = serviceDesc.tilematrixSets.front();
    REQUIRE(matrixSet.name == "GoogleMapsVL");
    REQUIRE(matrixSet.supportedCrs == "EPSG:3857");
    REQUIRE(matrixSet.bbox.crs == "EPSG:3857");
    REQUIRE(matrixSet.bbox.lowerCorner.longitude == Approx(-20037508.342789));
    REQUIRE(matrixSet.bbox.lowerCorner.latitude == Approx(-20037508.342789));
    REQUIRE(matrixSet.bbox.upperCorner.longitude == Approx(20037508.342789));
    REQUIRE(matrixSet.bbox.upperCorner.latitude == Approx(20037508.342789));
    REQUIRE(matrixSet.tileMatrices.size() == 22);

    auto* tileMatrix = matrixSet.tileMatrixWithId("0");
    REQUIRE_FALSE(tileMatrix == nullptr);
    REQUIRE(tileMatrix->id == "0");
    REQUIRE(tileMatrix->scaleDenominator == Approx(559082264.02871787548065185547));
    REQUIRE(tileMatrix->topLeftCorner.longitude == Approx(-20037508.342789));
    REQUIRE(tileMatrix->topLeftCorner.latitude == Approx(20037508.342789));
    REQUIRE(tileMatrix->matrixWidth == 1);
    REQUIRE(tileMatrix->matrixHeight == 1);
    REQUIRE(tileMatrix->tileWidth == 256);
    REQUIRE(tileMatrix->tileHeight == 256);

    tileMatrix = matrixSet.tileMatrixWithId("21");
    REQUIRE_FALSE(tileMatrix == nullptr);
    REQUIRE(tileMatrix->id == "21");
    REQUIRE(tileMatrix->scaleDenominator == Approx(266.59119798122287647857));
    REQUIRE(tileMatrix->topLeftCorner.longitude == Approx(-20037508.342789));
    REQUIRE(tileMatrix->topLeftCorner.latitude == Approx(20037508.342789));
    REQUIRE(tileMatrix->matrixWidth == 2097152);
    REQUIRE(tileMatrix->matrixHeight == 2097152);
    REQUIRE(tileMatrix->tileWidth == 256);
    REQUIRE(tileMatrix->tileHeight == 256);
    
    REQUIRE(matrixSet.tileMatrixWithId("21") == matrixSet.tileMatrixWithZoomLevel(21));
}

TEST_CASE("ConvertScaleToZoomLevel", "[conversion]")
{
    REQUIRE(wmts::scaleDenominatorToZoomLevel(559082264) == 0);
    REQUIRE(wmts::scaleDenominatorToZoomLevel(533) == 20);
}
}
