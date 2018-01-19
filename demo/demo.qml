import QtQuick 2.5
import QtQuick.Controls 2.3
import QtPositioning 5.5
import QtLocation 5.6

ApplicationWindow {
    //title of the application
    title: qsTr("WMTS Demo")
    width: 640
    height: 480
    visible: true

    Plugin {
        id: mapPlugin
        name: "wmts"
        // Disabled: required ssl libraries
//         PluginParameter {
//              name: "osm.mapping.highdpi_tiles"
//              value: true
//         }
        PluginParameter {
            name: "wmts.url"
            value: "http://tile.informatievlaanderen.be/ws/raadpleegdiensten/wmts"
        }
    }

    Map {
        id: flanders
        objectName: "map"
        property int visibleLayer: 0
        signal itemClicked(var index)
        
        anchors.fill: parent
        plugin: mapPlugin
        copyrightsVisible: false
        z: -1
        maximumZoomLevel: 22

        function setActiveMapType(typeName) {
            for (var i = 0; i < flanders.supportedMapTypes.length; i++) {
                var mapType = flanders.supportedMapTypes[i]
                if (mapType.name == typeName) {
                    flanders.activeMapType = mapType
                }
            }
        }

        function getMapTypes() {
            var mapTypeNames = []

            for (var i = 0; i < flanders.supportedMapTypes.length; ++i) {
                var mapType = flanders.supportedMapTypes[i]
                mapTypeNames.push(mapType.name)
            }
            return mapTypeNames
        }

        center {
            latitude: 51.095866571506406
            longitude: 4.23181677779314
        }
    }
}
