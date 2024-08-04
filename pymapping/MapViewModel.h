// geoint-mapping offers core mapping and Geospatial Intelligence capabilities for Python.
// Copyright (C) 2024 Jan Tschada (gisfromscratch@live.de)
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
// Additional permission under GNU LGPL version 3 section 4 and 5
// If you modify this Program, or any covered work, by linking or combining
// it with ArcGIS Runtime for Qt (or a modified version of that library),
// containing parts covered by the terms of ArcGIS Runtime for Qt,
// the licensors of this Program grant you additional permission to convey the resulting work.
// See <https://developers.arcgis.com/qt/> for further information.
//

#ifndef MAPVIEWMODEL_H
#define MAPVIEWMODEL_H

class SimpleGeoJsonLayer;

namespace Esri::ArcGISRuntime {
class Map;
class MapQuickView;
} // namespace Esri::ArcGISRuntime

#include <QObject>
#include <QList>
#include <QMouseEvent>

#include <Point.h>

Q_MOC_INCLUDE("MapQuickView.h")

class MapViewModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(Esri::ArcGISRuntime::MapQuickView *mapView READ mapView WRITE setMapView NOTIFY mapViewChanged)
    Q_PROPERTY(const QString& basemapStyle WRITE setBasemapStyle)
    Q_PROPERTY(const QString& mapViewExtent READ mapViewExtent WRITE setMapViewExtent NOTIFY mapViewExtentChanged)
    Q_PROPERTY(const QString& mapViewCenter READ mapViewCenter WRITE setMapViewCenter NOTIFY mapViewCenterChanged)

public:
    explicit MapViewModel(QObject *parent = nullptr);
    ~MapViewModel() override;

    void setBasemapStyle(const QString& basemapStyle);

    Q_INVOKABLE void loadBasemapFromTilePackage(const QString& tilePackageFilePath);
    Q_INVOKABLE void loadBasemapFromWMTS(const QString& wmtsServiceUrl, int layerIndex=0);

    Q_INVOKABLE void loadMapFromMobilePackage(const QString& mobileMapPackageFilePath, int mapIndex=0);

    Q_INVOKABLE bool addGeoJsonFeatures(const QString& features);
    Q_INVOKABLE bool addGeoJsonPointFeatures(const QString& features, const QString& renderer);
    Q_INVOKABLE bool addGeoJsonLineFeatures(const QString& features, const QString& renderer);
    Q_INVOKABLE bool addGeoJsonPolygonFeatures(const QString& features, const QString& renderer);

    Q_INVOKABLE void addFeatureLayer(const QString& featureServiceUrl);
    Q_INVOKABLE void addFeatureLayerFromMobile(const QString& workspacePath, const QString& featureClassName);
    Q_INVOKABLE void addFeatureLayerFromGeoPackage(const QString& workspacePath, const QString& featureClassName);

    Q_INVOKABLE void addRasterLayer(const QString& rasterFilePath, float opacity=0.7f);
    Q_INVOKABLE void addRasterLayerFromGeoPackage(const QString& workspacePath, const QString& rasterName, float opacity=0.7f);

    Q_INVOKABLE void clearGraphicOverlays();
    Q_INVOKABLE void clearOperationalLayers();

signals:
    void mapViewClicked(const QString& location);
    void mapViewChanged();
    void mapViewExtentChanged();
    void mapViewCenterChanged();

private slots:
    void onMouseClicked(QMouseEvent& mouseEvent);
    void onViewpointChanged();

private:
    Esri::ArcGISRuntime::MapQuickView *mapView() const;
    void setMapView(Esri::ArcGISRuntime::MapQuickView *mapView);

    QString mapViewExtent() const;
    void setMapViewExtent(const QString& extent);

    QString mapViewCenter() const;
    void setMapViewCenter(const QString& center);

    Esri::ArcGISRuntime::Map *m_map = nullptr;
    Esri::ArcGISRuntime::MapQuickView *m_mapView = nullptr;

    QList<SimpleGeoJsonLayer*> m_geojsonLayers;
};

#endif // MAPVIEWMODEL_H
