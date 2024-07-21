import os
import sys
from pathlib import Path

from PySide6.QtQml import QQmlApplicationEngine
from PySide6.QtGui import QGuiApplication
from PySide6.QtNetwork import QSslSocket
from PySide6.QtCore import QCoreApplication, QObject, QMetaObject, Qt, Q_RETURN_ARG, Q_ARG

import json

# Should include the directory containing the Qt libraries
# Take a look at the launch configuration (.vscode/launch.json)
print(os.environ["LD_LIBRARY_PATH"])

# The current Python environment must match the target Python release of the native coremapping library
# Otherwise the coremapping module cannot be imported
sys.path.append(os.path.join(Path(__file__).parent.parent, "build", "pymapping-Release"))
from coremapping import initialize


def find_mapview_model(window):
    window_children = window.findChildren(QObject)
    for window_child in window_children:
        if "MapViewModel" == window_child.metaObject().className():
            return window_child
        
    return None



if __name__ == "__main__":
    application = QGuiApplication(sys.argv)
    name = "Map Viewer example"
    QCoreApplication.setApplicationName(name)

    # Validate if the arcgis_api_key environment variable exists
    if not "arcgis_api_key" in os.environ:
        raise ValueError("'arcgis_api_key' is not defined in the current environment!")
    
    # Initializes the ArcGIS Runtime core environment
    # Also authenticate against ArcGIS Location Platform using an API Key
    api_key = os.environ.get("arcgis_api_key")
    initialize(api_key)

    engine = QQmlApplicationEngine()
    engine.rootContext().setContextProperty("supportsSsl", QSslSocket.supportsSsl())
    engine.addImportPath(Path(__file__).parent)
    engine.loadFromModule("MapViewer", "Main")
    engine.quit.connect(QCoreApplication.quit)

    items = engine.rootObjects()
    if not items:
        sys.exit(-1)

    context = engine.rootContext()
    appWindow = items[0]
    mapview_model = find_mapview_model(appWindow)
    if not mapview_model:
        sys.exit(-1)

    # Invoke custom method using the map view model
    features = {
        "type": "FeatureCollection",
        "name": "berlin_start_locations",
        "crs": { "type": "name", "properties": { "name": "urn:ogc:def:crs:OGC:1.3:CRS84" } },
        "features": [
            { "type": "Feature", "properties": { "name": "U-Bahn Ruhleben" }, "geometry": { "type": "Point", "coordinates": [ 13.241183855834903, 52.525492469777419 ] } },
            { "type": "Feature", "properties": { "name": "U-Bahn Olympiastadion" }, "geometry": { "type": "Point", "coordinates": [ 13.249940139300833, 52.516993066967835 ] } },
            { "type": "Feature", "properties": { "name": "S-Bahn Olympiastadion" }, "geometry": { "type": "Point", "coordinates": [ 13.241096425470483, 52.511158549082381 ] } },
            { "type": "Feature", "properties": { "name": "S-Bahn Pichelsberg" }, "geometry": { "type": "Point", "coordinates": [ 13.22863185816456, 52.509811598401058 ] } },
            { "type": "Feature", "properties": { "name": "S-Bahn Stresow" }, "geometry": { "type": "Point", "coordinates": [ 13.20952081689347, 52.532019724249047 ] } }
        ]
    }
    succeeded = QMetaObject.invokeMethod(mapview_model, "addFeatureLayer",
        Qt.DirectConnection,
        Q_RETURN_ARG(bool),
        Q_ARG(str, json.dumps(features)))

    ex = application.exec()
    del engine
    sys.exit(ex)