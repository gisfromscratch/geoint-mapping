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
from coremapping import initialize, MapViewModel


def find_mapview_model(window) -> MapViewModel:
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
    features = {
        "type": "FeatureCollection",
        "name": "polygones",
        "crs": { "type": "name", "properties": { "name": "urn:ogc:def:crs:OGC:1.3:CRS84" } },
        "features": [
            { "type": "Feature", "properties": { "description": "" }, "geometry": { "type": "Polygon", "coordinates": [ [ [ 12.22827640517508, 51.828540205355615 ], [ 12.242468341563754, 51.830753357651197 ], [ 12.234908151151094, 51.839604879361012 ], [ 12.216538214797529, 51.839440978104179 ], [ 12.22827640517508, 51.828540205355615 ] ] ] } }
        ]
    }
    features = {
        "type": "FeatureCollection",
        "name": "multipolygons",
        "crs": { "type": "name", "properties": { "name": "urn:ogc:def:crs:OGC:1.3:CRS84" } },
        "features": [
            { "type": "Feature", "properties": { "description": "" }, "geometry": { "type": "MultiPolygon", "coordinates": [ [ [ [ 12.228486808643686, 51.828495605171504 ], [ 12.24225381899827, 51.830815978045706 ], [ 12.235022662044347, 51.839580752319037 ], [ 12.216944769659541, 51.839408910397175 ], [ 12.228486808643686, 51.828495605171504 ] ], [ [ 12.236065617374237, 51.833136231378248 ], [ 12.228486808643686, 51.831761280870595 ], [ 12.223063440928245, 51.837131943013667 ], [ 12.231615674633364, 51.837604530619146 ], [ 12.236065617374237, 51.833136231378248 ] ] ] ] } },
            { "type": "Feature", "properties": { "description": "" }, "geometry": { "type": "MultiPolygon", "coordinates": [ [ [ [ 12.215484632197692, 51.826604842590655 ], [ 12.228625869354339, 51.828581547039349 ], [ 12.231476613922712, 51.817407729173226 ], [ 12.224732169456074, 51.815946332694999 ], [ 12.221672833821721, 51.812937425998243 ], [ 12.215623692908341, 51.81328131121753 ], [ 12.215484632197692, 51.826604842590655 ] ] ] ] } },
            { "type": "Feature", "properties": { "description": "" }, "geometry": { "type": "MultiPolygon", "coordinates": [ [ [ [ 12.238985892297936, 51.848000203064764 ], [ 12.232171917475972, 51.846282075700202 ], [ 12.235370313820976, 51.842931538703837 ], [ 12.237734345902064, 51.844048412078763 ], [ 12.238985892297936, 51.848000203064764 ] ] ] ] } }
        ]
    }

    renderer = {'labelingInfo': None,
            'label': '',
            'description': '',
            'type': 'simple',
            'symbol': {'type': 'esriSFS',
            'style': 'esriSFSSolid',
            'color': [0, 128, 0, 128],
            'outline': {'type': 'esriSLS',
            'style': 'esriSLSSolid',
            'color': [110, 110, 110, 255],
            'width': 1}}}
    
    mapview_model.addGeoJsonPolygonFeatures(json.dumps(features), json.dumps(renderer))

    """
    succeeded = QMetaObject.invokeMethod(mapview_model, "addGeoJsonPolygonFeatures",
        Qt.DirectConnection,
        Q_RETURN_ARG(bool),
        Q_ARG(str, json.dumps(features)),
        Q_ARG(str, json.dumps(renderer)))
    """

    ex = application.exec()
    del engine
    sys.exit(ex)