# To be used on the @QmlElement decorator
# (QML_IMPORT_MINOR_VERSION is optional)
QML_IMPORT_NAME = "esri.mapping.slots"
QML_IMPORT_MAJOR_VERSION = 1

from PySide6.QtQml import QmlElement
from PySide6.QtCore import QObject, Slot

from coremapping import GeoElementsOverlayModel


@QmlElement
class Bridge(QObject):

    @Slot(str, result=str)
    def getValue(self, input):
        return f"#{input}"
    
@QmlElement
class GeoElementsInspector(QObject):

    @Slot(QObject)
    def inspect(self, model: GeoElementsOverlayModel):
        if 0 < model.getCount():
            geoelements = model.toDict(0)
            for index in range(0, len(geoelements)):
                geoelement = geoelements[index]
                for key, value in geoelement.items():
                    print(key, ":", value)
