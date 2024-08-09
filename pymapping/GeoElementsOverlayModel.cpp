#include "GeoElementsOverlayModel.h"

#include <AttributeListModel.h>
#include <GeoElement.h>
#include <Geometry.h>
#include <Graphic.h>
#include <GraphicListModel.h>
#include <GraphicsOverlay.h>
#include <GraphicsOverlayListModel.h>

using namespace Esri::ArcGISRuntime;

GeoElementsOverlayModel::GeoElementsOverlayModel(QObject *parent)
    : QObject{parent}
{
}

void GeoElementsOverlayModel::init(GraphicsOverlayListModel* overlayListModel)
{
    m_overlayListModel = overlayListModel;
}

int GeoElementsOverlayModel::count() const
{
    return m_overlayListModel->rowCount();
}

QVariantList GeoElementsOverlayModel::toDict(int index) const
{
    QVariantList geoElements;
    if (index < 0 || this->count() <= index)
    {
        return geoElements;
    }

    GraphicsOverlay* overlay = m_overlayListModel->at(index);
    GraphicListModel* graphics = overlay->graphics();
    QList<GeoElement*> graphicsList;
    graphicsList.reserve(graphics->size());
    std::for_each(graphics->begin(), graphics->end(), [&geoElements](Graphic* graphic)
    {
        AttributeListModel* attributes = graphic->attributes();
        QVariantMap attributesMap = attributes->attributesMap();
        Geometry geometry = graphic->geometry();
        attributesMap.insert("geometry", geometry.toJson());
        geoElements.append(attributesMap);
    });

    return geoElements;
}
