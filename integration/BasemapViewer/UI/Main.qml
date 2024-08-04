import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

import Esri.Mapping

ApplicationWindow {
    visible: true
    width: 800
    height: 600

    Material.theme: Material.Dark
    Material.accent: "#C9F2FF"
    Material.background: "#0289C3"
    Material.foreground: "#FFFFFF"
    Material.primary: "#035799"

    font.pixelSize: 14

    footer: TabBar {

        TabButton {
            text: qsTr("Prev")
            onClicked: {
                if (0 < basemapIndex) {
                    basemapIndex--;
                    var basemapStyle = basemapStyles[basemapIndex];                
                    mapViewComponent.setBasemapStyle(basemapStyle);
                }
            }
        }

        TabButton {
            text: qsTr("Next")
            onClicked: {
                if (basemapIndex + 1 < basemapStyles.length) {
                    basemapIndex++;
                    var basemapStyle = basemapStyles[basemapIndex];                
                    mapViewComponent.setBasemapStyle(basemapStyle);
                }
            }
        }
    }

    property list<string> basemapStyles: [
        "ArcGISImagery",
        "ArcGISLightGray",
        "ArcGISDarkGray",
        "ArcGISNavigation",
        "ArcGISStreets",
        "ArcGISTopographic",
        "ArcGISTerrain",
        "ArcGISCommunity",
        "OsmStandard"
    ]

    property int basemapIndex: 0

    MapViewComponent {
        id: mapViewComponent
        anchors.fill: parent
    }
}