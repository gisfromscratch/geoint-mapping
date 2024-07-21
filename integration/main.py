import os
import sys
from pathlib import Path

from PySide6.QtQml import QQmlApplicationEngine
from PySide6.QtGui import QGuiApplication
from PySide6.QtNetwork import QSslSocket
from PySide6.QtCore import QCoreApplication

# Should include the directory containing the Qt libraries
# Take a look at the launch configuration (.vscode/launch.json)
print(os.environ["LD_LIBRARY_PATH"])

# The current Python environment must match the target Python release of the native coremapping library
# Otherwise the coremapping module cannot be imported
sys.path.append(os.path.join(Path(__file__).parent.parent, "build", "pymapping-Release"))
from coremapping import initialize


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

    ex = application.exec()
    del engine
    sys.exit(ex)