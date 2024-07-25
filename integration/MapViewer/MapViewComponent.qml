import QtQuick
import QtQuick.Controls
import Esri.Mapping

import esri.mapping.slots

Item {

    // Create MapQuickView here, and create its Map etc. in C++ code
    MapView {
        id: view
        anchors.fill: parent
        // set focus to enable keyboard navigation
        focus: true
    }

    Bridge {
        id: bridge
    }

    // Declare the C++ instance which creates the map etc. and supply the view
    MapViewModel {
        id: model
        mapView: view
        basemapStyle: "OsmStandard"
        
        onMapViewChanged: {
            console.log(bridge.getValue("Map view changed"));
        }

        onMapViewExtentChanged: {
            console.log(model.mapViewExtent);
        }

        onMapViewClicked: location => {
            console.log(location);
            model.clearGraphicOverlays();
            model.mapViewExtent = '{"xmin":1355768.573867436,"ymin":6764128.1390294079,"xmax":1366278.4719742704,"ymax":6772010.5626095338,"spatialReference":{"wkid":102100,"latestWkid":3857}}';
        }
    }
}