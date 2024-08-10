#ifndef GEOELEMENTOVERLAYMODEL_H
#define GEOELEMENTOVERLAYMODEL_H

#include <QObject>
#include <QVariantList>
#include <QVariantMap>
#include <QQmlEngine>

namespace Esri::ArcGISRuntime {
class GraphicsOverlayListModel;
} // namespace Esri::ArcGISRuntime

class GeoElementsOverlayModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int count READ count CONSTANT)

public:
    explicit GeoElementsOverlayModel(QObject *parent = nullptr);

    void init(Esri::ArcGISRuntime::GraphicsOverlayListModel* overlayListModel);

    int count() const;

    Q_INVOKABLE QVariantList toDict(int index) const;

signals:

private:
    Esri::ArcGISRuntime::GraphicsOverlayListModel* m_overlayListModel;
};

Q_DECLARE_METATYPE(GeoElementsOverlayModel)

#endif // GEOELEMENTOVERLAYMODEL_H
