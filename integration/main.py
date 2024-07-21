import os
import sys
from pathlib import Path

from PySide6.QtQml import QQmlApplicationEngine
from PySide6.QtGui import QGuiApplication
from PySide6.QtNetwork import QSslSocket
from PySide6.QtCore import QCoreApplication, QMetaObject, Q_ARG

print(os.environ['LD_LIBRARY_PATH'])

from mapping import initialize


if __name__ == "__main__":
    additionalLibraryPaths = os.environ.get("QTLOCATION_EXTRA_LIBRARY_PATH")
    if additionalLibraryPaths:
        for p in additionalLibraryPaths.split(':'):
            QCoreApplication.addLibraryPath(p)

    application = QGuiApplication(sys.argv)
    name = "Map Viewer example"
    QCoreApplication.setApplicationName(name)

    initialize()

    engine = QQmlApplicationEngine()
    #engine.rootContext().setContextProperty("supportsSsl", QSslSocket.supportsSsl())
    engine.addImportPath(Path(__file__).parent)
    engine.loadFromModule("MapViewer", "Main")
    engine.quit.connect(QCoreApplication.quit)

    items = engine.rootObjects()
    if not items:
        sys.exit(-1)

    ex = application.exec()
    del engine
    sys.exit(ex)