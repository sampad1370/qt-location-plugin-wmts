#include "serviceparser.h"

#include <array>
#include <cstdio>
#include <iostream>
#include <pugixml.hpp>
#include <sstream>

namespace wmts
{

using namespace pugi;

static constexpr std::array<int, 23> s_zoomLevelLut{{
    559'082'264,
    279'541'132,
    139'770'566,
    69'885'283,
    34'942'642,
    17'471'321,
    8'735'660,
    4'367'830,
    2'183'915,
    1'091'958,
    545'979,
    272'989,
    136'495,
    68'247,
    34'124,
    17'062,
    8'531,
    4'265,
    2'133,
    1'066,
    533,
    266,
    133
}};

static Coordinate parseCoordinateNode(const xml_node& node)
{
    Coordinate coord;
    std::sscanf(node.child_value(), "%lf %lf", &coord.longitude, &coord.latitude);
    return coord;
}

static Layer parseLayerNode(const xml_node& layerNode)
{
    Layer layer;
    layer.name        = layerNode.child("ows:Title").child_value();
    layer.description = layerNode.child("ows:Abstract").child_value();
    layer.format      = layerNode.child("Format").child_value();

    auto bboxNode          = layerNode.child("ows:WGS84BoundingBox");
    layer.bbox.upperCorner = parseCoordinateNode(bboxNode.child("ows:UpperCorner"));
    layer.bbox.lowerCorner = parseCoordinateNode(bboxNode.child("ows:LowerCorner"));
    layer.bbox.crs         = "EPSG:4326";
    
    for (auto& styleNode : layerNode.children("Style"))
    {
        layer.styles.emplace_back(styleNode.child_value("ows:Title"));
        if (styleNode.attribute("isDefault").as_bool())
        {
            layer.defaultStyle = layer.styles.back();
        }
    }

    for (auto& matrixSetLinkNode : layerNode.children("TileMatrixSetLink"))
    {
        TileMatrixSetLink matrixSetLink;
        matrixSetLink.tileMatrixSet = matrixSetLinkNode.child_value("TileMatrixSet");
        matrixSetLink.minZoomLevel  = std::numeric_limits<int>::max();
        matrixSetLink.maxZoomLevel  = std::numeric_limits<int>::lowest();

        for (auto& matrixSetLimitsNode : matrixSetLinkNode.child("TileMatrixSetLimits").children("TileMatrixLimits"))
        {
            TileMatrixLimits limits;
            if (std::sscanf(matrixSetLimitsNode.child_value("TileMatrix"), "%*[^:]:%d", &limits.zoomLevel) != 1)
            {
                limits.zoomLevel = -1;
            }

            limits.colRange.min        = matrixSetLimitsNode.child("MinTileCol").first_child().text().as_int();
            limits.colRange.max        = matrixSetLimitsNode.child("MaxTileCol").first_child().text().as_int();
            limits.rowRange.min        = matrixSetLimitsNode.child("MinTileRow").first_child().text().as_int();
            limits.rowRange.max        = matrixSetLimitsNode.child("MaxTileRow").first_child().text().as_int();
            matrixSetLink.minZoomLevel = std::min(matrixSetLink.minZoomLevel, limits.zoomLevel);
            matrixSetLink.maxZoomLevel = std::max(matrixSetLink.maxZoomLevel, limits.zoomLevel);
            matrixSetLink.limits.push_back(limits);
        }

        std::sort(begin(matrixSetLink.limits), end(matrixSetLink.limits), [](const auto& l1, const auto& l2) {
            return l1.zoomLevel < l2.zoomLevel;
        });

        layer.tileMatrixSetLinks.emplace_back(std::move(matrixSetLink));
    }

    return layer;
}

static TileMatrixSet parseMatrixSetNode(const xml_node& matrixSetNode)
{
    TileMatrixSet matrixSet;
    matrixSet.name         = matrixSetNode.child_value("ows:Identifier");
    matrixSet.supportedCrs = matrixSetNode.child_value("ows:SupportedCRS");

    auto bboxNode              = matrixSetNode.child("ows:BoundingBox");
    matrixSet.bbox.upperCorner = parseCoordinateNode(bboxNode.child("ows:UpperCorner"));
    matrixSet.bbox.lowerCorner = parseCoordinateNode(bboxNode.child("ows:LowerCorner"));
    matrixSet.bbox.crs         = bboxNode.attribute("crs").as_string();

    for (auto& matrixNode : matrixSetNode.children("TileMatrix"))
    {
        TileMatrix matrix;
        matrix.id               = matrixNode.child_value("ows:Identifier");
        matrix.scaleDenominator = std::atof(matrixNode.child_value("ScaleDenominator"));
        matrix.tileWidth        = std::atoi(matrixNode.child_value("TileWidth"));
        matrix.tileHeight       = std::atoi(matrixNode.child_value("TileHeight"));
        matrix.matrixWidth      = std::atoi(matrixNode.child_value("MatrixWidth"));
        matrix.matrixHeight     = std::atoi(matrixNode.child_value("MatrixHeight"));
        matrix.topLeftCorner    = parseCoordinateNode(matrixNode.child("TopLeftCorner"));

        matrixSet.tileMatrices.emplace_back(std::move(matrix));
    }

    return matrixSet;
}

WmtsServiceDescription parseServiceDescription(std::string_view data)
{
    WmtsServiceDescription desc;

    xml_document doc;
    if (doc.load_buffer(data.data(), data.size()).status != xml_parse_status::status_ok)
    {
        throw std::runtime_error("Failed to parse wmts service description");
    }

    auto caps    = doc.child("Capabilities");
    desc.version = caps.attribute("version").as_string();
    desc.name    = caps.child("ows:ServiceIdentification").child("ows:Title").child_value();

    auto contents = caps.child("Contents");
    for (auto& layerNode : contents.children("Layer"))
    {
        desc.layers.emplace_back(parseLayerNode(layerNode));
    }

    for (auto& matrixSetNode : contents.children("TileMatrixSet"))
    {
        desc.tilematrixSets.emplace_back(parseMatrixSetNode(matrixSetNode));
    }

    return desc;
}

TileMatrix* TileMatrixSet::tileMatrixWithId(std::string_view name)
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

TileMatrix* TileMatrixSet::tileMatrixWithZoomLevel(int zoomLevel)
{
    if (zoomLevel >= int(s_zoomLevelLut.size()))
    {
        return nullptr;
    }

    int scale = s_zoomLevelLut[zoomLevel];
    auto iter = std::find_if(begin(tileMatrices), end(tileMatrices), [scale](const auto& tm) {
        return int(tm.scaleDenominator) == scale;
    });

    if (iter == end(tileMatrices))
    {
        return nullptr;
    }

    return &(*iter);
}


int scaleDenominatorToZoomLevel(int scale)
{
    auto iter = std::find(begin(s_zoomLevelLut), end(s_zoomLevelLut), scale);
    if (iter == end(s_zoomLevelLut)) {
        return -1;
    }

    return int(std::distance(begin(s_zoomLevelLut), iter));
}
}
