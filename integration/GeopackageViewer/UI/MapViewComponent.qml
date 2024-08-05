import QtQuick
import QtQuick.Controls
import Esri.Mapping

Item {

    // Create MapQuickView here, and create its Map etc. in C++ code
    MapView {
        id: view
        anchors.fill: parent
        // set focus to enable keyboard navigation
        focus: true
    }

    DropArea {
        anchors.fill: parent

        onDropped: (drop) => {
            function readGeopackage(fileUrl) {
                var filePath = fileUrl.toString();
                filePath = filePath.replace(/^.*?:(\/){2,}/, "/");
                if (filePath.endsWith(".gpkg")) {
                    console.log(`Reading features from ${filePath}`);
                    model.addFeatureLayerFromGeoPackage(filePath, "");
                }
            }
            drop.urls.forEach(readGeopackage);
        }
    }

    // Declare the C++ instance which creates the map etc. and supply the view
    MapViewModel {
        id: model
        mapView: view
        basemapStyle: "OsmStandard"

        onMapViewClicked: location => {
            model.clearOperationalLayers();
        }
    }
}