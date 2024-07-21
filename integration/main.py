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
from coremapping import initialize, BasemapStyle


if __name__ == "__main__":
    application = QGuiApplication(sys.argv)
    name = "Map Viewer example"
    QCoreApplication.setApplicationName(name)

    initialize()
    basemap_style = BasemapStyle.ArcGISImagery

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