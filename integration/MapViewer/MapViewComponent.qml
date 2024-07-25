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

        onMapViewClicked: location => {
            console.log(location);
        }
    }
}