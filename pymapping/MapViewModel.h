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

class GeoElementsOverlayModel;
class SimpleGeoJsonLayer;

namespace Esri::ArcGISRuntime {
class GeometryEditor;
class GraphicsOverlay;
class Map;
class MapQuickView;
class VertexTool;
} // namespace Esri::ArcGISRuntime

#include <QObject>
#include <QList>
#include <QMouseEvent>

#include <Point.h>

Q_MOC_INCLUDE("MapQuickView.h")
Q_MOC_INCLUDE("GeoElementsOverlayModel.h")

class MapViewModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(Esri::ArcGISRuntime::MapQuickView *mapView READ mapView WRITE setMapView NOTIFY mapViewChanged)
    Q_PROPERTY(const QString& basemapStyle WRITE setBasemapStyle)
    Q_PROPERTY(const QString& mapViewExtent READ mapViewExtent WRITE setMapViewExtent NOTIFY mapViewExtentChanged)
    Q_PROPERTY(const QString& mapViewCenter READ mapViewCenter WRITE setMapViewCenter NOTIFY mapViewCenterChanged)
    Q_PROPERTY(GeoElementsOverlayModel* overlayModel READ overlayModel CONSTANT)

public:
    explicit MapViewModel(QObject *parent = nullptr);
    ~MapViewModel() override;

    enum class SketchEditorMode {
        PointSketchMode,
        MultipointSketchMode,
        PolylineSketchMode,
        PolygonSketchMode
    };

    Q_ENUM(SketchEditorMode)

    void setBasemapStyle(const QString& basemapStyle);

    Q_INVOKABLE void loadBasemapFromTilePackage(const QString& tilePackageFilePath);
    Q_INVOKABLE void loadBasemapFromWMTS(const QString& wmtsServiceUrl, int layerIndex=0);

    Q_INVOKABLE void loadMapFromMobilePackage(const QString& mobileMapPackageFilePath, int mapIndex=0);

    Q_INVOKABLE bool addGeoJsonFeatures(const QString& features);
    Q_INVOKABLE bool addGeoJsonPointFeatures(const QString& features, const QString& renderer);
    Q_INVOKABLE bool addGeoJsonLineFeatures(const QString& features, const QString& renderer);
    Q_INVOKABLE bool addGeoJsonPolygonFeatures(const QString& features, const QString& renderer);

    Q_INVOKABLE void addGeometries(const QString& geometries, const QString& renderer);

    Q_INVOKABLE void addFeatureLayer(const QString& featureServiceUrl);
    Q_INVOKABLE void addFeatureLayerFromMobile(const QString& workspacePath, const QString& featureClassName);
    Q_INVOKABLE void addFeatureLayerFromGeoPackage(const QString& workspacePath, const QString& featureClassName);

    Q_INVOKABLE void addRasterLayer(const QString& rasterFilePath, float opacity=0.7f);
    Q_INVOKABLE void addRasterLayerFromGeoPackage(const QString& workspacePath, const QString& rasterName, float opacity=0.7f);

    Q_INVOKABLE void clearGraphicOverlays();
    Q_INVOKABLE void clearOperationalLayers();    

    Q_INVOKABLE void startSketching(SketchEditorMode sketchEditorMode);
    Q_INVOKABLE void stopSketching();

signals:
    void mapViewClicked(const QString& location);
    void mapViewChanged();
    void mapViewExtentChanged();
    void mapViewCenterChanged();
    void sketchCompleted(const QString& geometry);

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

    GeoElementsOverlayModel* overlayModel() const;

    Esri::ArcGISRuntime::Map *m_map = nullptr;
    Esri::ArcGISRuntime::MapQuickView *m_mapView = nullptr;
    Esri::ArcGISRuntime::GeometryEditor *m_geometryEditor = nullptr;
    Esri::ArcGISRuntime::VertexTool *m_sketchTool = nullptr;

    QList<SimpleGeoJsonLayer*> m_geojsonLayers;
    QList<Esri::ArcGISRuntime::GraphicsOverlay*> m_graphicLayers;
    GeoElementsOverlayModel* m_overlayModel;
};

#endif // MAPVIEWMODEL_H
