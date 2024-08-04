import QtQuick
import QtQuick.Controls
import Esri.Mapping

Item {

    function setBasemapStyle(basemapStyle) {
        model.basemapStyle = basemapStyle;
    }

    // Create MapQuickView here, and create its Map etc. in C++ code
    MapView {
        id: view
        anchors.fill: parent
        // set focus to enable keyboard navigation
        focus: true
    }

    // Declare the C++ instance which creates the map etc. and supply the view
    MapViewModel {
        id: model
        mapView: view
        basemapStyle: "ArcGISImagery"
    }
}