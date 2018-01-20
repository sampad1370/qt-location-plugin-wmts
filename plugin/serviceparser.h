#pragma once

#include <string>
#include <vector>
#include <cinttypes>

#include <string_view>

namespace wmts {

struct Range
{
    int min;
    int max;
};

struct Coordinate
{
    double longitude;
    double latitude;
};

struct BoundingBox
{
    std::string crs;
    Coordinate lowerCorner;
    Coordinate upperCorner;
};

struct TileMatrixLimits
{
    int zoomLevel;
    Range rowRange;
    Range colRange;
};

struct TileMatrix
{
    int id;
    double scaleDenominator;
    Coordinate topLeftCorner;
    int tileWidth;
    int tileHeight;
    int matrixWidth = 1;
    int matrixHeight = 1;
};

struct TileMatrixSet
{
    std::string name;
    std::string supportedCrs;
    BoundingBox bbox;
    std::vector<TileMatrix> tileMatrices;
};

struct TileMatrixSetLink
{
    std::string tileMatrixSet;
    int minZoomLevel;
    int maxZoomLevel;
    std::vector<TileMatrixLimits> limits;
};

struct Layer
{
    std::string name;
    std::string description;
    std::string format;
    BoundingBox bbox;
    std::vector<TileMatrixSetLink> tileMatrixSets;
};

struct WmtsServiceDescription
{
    std::string version;
    std::string name;
    
    std::vector<Layer> layers;
    std::vector<TileMatrixSet> tilematrixSets;
};

WmtsServiceDescription parseServiceDescription(std::string_view data);

}
