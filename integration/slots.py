# To be used on the @QmlElement decorator
# (QML_IMPORT_MINOR_VERSION is optional)
QML_IMPORT_NAME = "esri.mapping.slots"
QML_IMPORT_MAJOR_VERSION = 1

from PySide6.QtQml import QmlElement
from PySide6.QtCore import QObject, Slot


@QmlElement
class Bridge(QObject):

    @Slot(str, result=str)
    def getValue(self, input):
        return f"#{input}"