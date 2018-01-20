#pragma once

#include <cassert>
#include <cinttypes>
#include <string>
#include <vector>

#include <string_view>

namespace wmts
{

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
    Coordinate  lowerCorner;
    Coordinate  upperCorner;
};

struct TileMatrixLimits
{
    int   zoomLevel;
    Range rowRange;
    Range colRange;
};

struct TileMatrix
{
    std::string id;
    double      scaleDenominator;
    Coordinate  topLeftCorner;
    int         tileWidth;
    int         tileHeight;
    int         matrixWidth  = 1;
    int         matrixHeight = 1;
};

struct TileMatrixSet
{
    TileMatrix* getTileMatrix(std::string_view name)
    {
        auto iter = std::find_if(begin(tileMatrices), end(tileMatrices), [=](const auto& tm) {
            return tm.id == name;
        });

        if (iter == end(tileMatrices))
        {
            return nullptr;
        }

        return &(*iter);
    }

    std::string             name;
    std::string             supportedCrs;
    BoundingBox             bbox;
    std::vector<TileMatrix> tileMatrices;
};

struct TileMatrixSetLink
{
    TileMatrixLimits& getTileMatrixLimits(int zoomLevel)
    {
        assert(zoomLevel >= minZoomLevel && zoomLevel <= maxZoomLevel);
        return limits[zoomLevel - minZoomLevel];
    }

    std::string                   tileMatrixSet;
    int                           minZoomLevel;
    int                           maxZoomLevel;
    std::vector<TileMatrixLimits> limits;
};

struct Layer
{
    std::string                    name;
    std::string                    description;
    std::string                    format;
    BoundingBox                    bbox;
    std::vector<TileMatrixSetLink> tileMatrixSetLinks;
};

struct WmtsServiceDescription
{
    std::string version;
    std::string name;

    std::vector<Layer>         layers;
    std::vector<TileMatrixSet> tilematrixSets;
};

WmtsServiceDescription parseServiceDescription(std::string_view data);
int                    scaleDenominatorToZoomLevel(int scale);
}
