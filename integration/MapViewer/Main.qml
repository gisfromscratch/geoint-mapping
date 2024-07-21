import QtQuick.Controls
import Esri.Mapping

ApplicationWindow {
    visible: true
    width: 800
    height: 600

    MapViewComponent {
        id: mapViewComponent
        anchors.fill: parent
    }
}