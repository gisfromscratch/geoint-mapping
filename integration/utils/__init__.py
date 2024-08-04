import os
import sys

from PySide6.QtGui import QGuiApplication
from PySide6.QtQml import QQmlApplicationEngine
from PySide6.QtNetwork import QSslSocket
from PySide6.QtCore import QCoreApplication, QObject


def add_module_directories():
    """
    Extends the path using the environment variable named 'maptoolkit_path'.
    Raises a ValueError if this environment variable does not exists!
    """
    # Since Python 3.8 add_dll_directory is necessary!
    if not "maptoolkit_path" in os.environ:
        raise ValueError("'maptoolkit_path' is not defined in the current environment!")
    for native_dir in os.environ["maptoolkit_path"].split(";"):
        try:
            # Only works on Windows!
            os.add_dll_directory(native_dir)
        except:
            pass
        sys.path.append(native_dir)

def initialize_arcgis():
    """
    Initializes the ArcGIS core environment.
    Authenticates against ArcGIS Location Platform using the environment variable named 'arcgis_api_key'.
    Raises a ValueError if this environment variable does not exists!
    """
    # Validate if the arcgis_api_key environment variable exists
    if not "arcgis_api_key" in os.environ:
        raise ValueError("'arcgis_api_key' is not defined in the current environment!")
    
    # Initializes the ArcGIS Runtime core environment
    # Also authenticate against ArcGIS Location Platform using an API Key
    api_key = os.environ.get("arcgis_api_key")

    from coremapping import initialize
    initialize(api_key)

def create_quick_app(app_name: str, app_folder: str) -> tuple[QGuiApplication, QQmlApplicationEngine]:
    """
    Creates a Qt Quick based app using an app folder containing a UI component named Main.
    :param app_name: The name of the app.
    :param app_folder: The name of the folder containing the UI. 

    Returns the application and the QML app engine as a tuple (app, engine)
    Raises a ValueError if the QML engine was not able to load any UI components!
    """
    application = QGuiApplication(sys.argv)    
    QCoreApplication.setApplicationName(app_name)

    engine = QQmlApplicationEngine()
    engine.rootContext().setContextProperty("supportsSsl", QSslSocket.supportsSsl())
    engine.addImportPath(app_folder)
    engine.loadFromModule("UI", "Main")
    engine.quit.connect(QCoreApplication.quit)

    items = engine.rootObjects()
    if not items:
        raise ValueError("The QML engine was not able to load any UI component!")

    return (application, engine)

def find_mapview_model(window):
    """
    Tries to identify the underlying MapViewModel using a root app window.

    :param window: The root app window.
    """
    window_children = window.findChildren(QObject)
    for window_child in window_children:
        if "MapViewModel" == window_child.metaObject().className():
            return window_child
        
    return None