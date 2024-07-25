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

#include "MapViewModel.h"

#include <algorithm>

#include <QJsonDocument>

#include <Basemap.h>
#include <FeatureCollection.h>
#include <FeatureCollectionLayer.h>
#include <FeatureCollectionTable.h>
#include <FeatureCollectionTableListModel.h>
#include <Field.h>
#include <GeometryTypes.h>
#include <Graphic.h>
#include <GraphicListModel.h>
#include <GraphicsOverlay.h>
#include <GraphicsOverlayListModel.h>
#include <LayerListModel.h>
#include <Map.h>
#include <MapQuickView.h>
#include <MapTypes.h>
#include <Renderer.h>
#include <SpatialReference.h>

#include "SimpleGeoJsonLayer.h"

using namespace Esri::ArcGISRuntime;

MapViewModel::MapViewModel(QObject *parent /* = nullptr */)
    : QObject(parent)
    , m_map(new Map(BasemapStyle::ArcGISStreets, this))
{
    qDebug() << "Map view model was instantiated.";
}

MapViewModel::~MapViewModel() {}

MapQuickView *MapViewModel::mapView() const
{
    return m_mapView;
}

// Set the view (created in QML)
void MapViewModel::setMapView(MapQuickView *mapView)
{
    if (!mapView || mapView == m_mapView) {
        return;
    }

    if (m_mapView)
    {
        disconnect(m_mapView, &MapQuickView::mouseClicked, this, &MapViewModel::onMouseClicked);
    }

    m_mapView = mapView;
    m_mapView->setMap(m_map);
    qDebug() << "Map view model was updated with a new map view";

    connect(m_mapView, &MapQuickView::mouseClicked, this, &MapViewModel::onMouseClicked);

    emit mapViewChanged();
}

void MapViewModel::setBasemapStyle(const QString& basemapStyle)
{
    if (m_map)
    {
        BasemapStyle newBasemapStyle;
        bool supportedBasemapStyle = false;
        if ("ArcGISImagery" == basemapStyle)
        {
            newBasemapStyle = BasemapStyle::ArcGISImagery;
            supportedBasemapStyle = true;
        }
        else if ("ArcGISLightGray" == basemapStyle)
        {
            newBasemapStyle = BasemapStyle::ArcGISLightGray;
            supportedBasemapStyle = true;
        }
        else if ("ArcGISDarkGray" == basemapStyle)
        {
            newBasemapStyle = BasemapStyle::ArcGISDarkGray;
            supportedBasemapStyle = true;
        }
        else if ("ArcGISNavigation" == basemapStyle)
        {
            newBasemapStyle = BasemapStyle::ArcGISNavigation;
            supportedBasemapStyle = true;
        }
        else if ("ArcGISStreets" == basemapStyle)
        {
            newBasemapStyle = BasemapStyle::ArcGISStreets;
            supportedBasemapStyle = true;
        }
        else if ("ArcGISTopographic" == basemapStyle)
        {
            newBasemapStyle = BasemapStyle::ArcGISTopographic;
            supportedBasemapStyle = true;
        }
        else if ("ArcGISOceans" == basemapStyle)
        {
            newBasemapStyle = BasemapStyle::ArcGISOceans;
            supportedBasemapStyle = true;
        }
        else if ("ArcGISTerrain" == basemapStyle)
        {
            newBasemapStyle = BasemapStyle::ArcGISTerrain;
            supportedBasemapStyle = true;
        }
        else if ("ArcGISCommunity" == basemapStyle)
        {
            newBasemapStyle = BasemapStyle::ArcGISCommunity;
            supportedBasemapStyle = true;
        }
        else if ("OsmStandard" == basemapStyle)
        {
            newBasemapStyle = BasemapStyle::OsmStandard;
            supportedBasemapStyle = true;
        }

        if (supportedBasemapStyle)
        {
            m_map->setBasemap(new Basemap(newBasemapStyle, this));
        }
        else
        {
            qWarning() << basemapStyle << "is not a supported basemap style!";
        }
    }
}

bool MapViewModel::addGeoJsonFeatures(const QString& features)
{
    qDebug() << "Try to add GeoJSON features as feature layers...";
    //qDebug() << features;

    SimpleGeoJsonLayer* geojsonLayer = new SimpleGeoJsonLayer(this);
    QJsonDocument geojsonDocument = QJsonDocument::fromJson(features.toUtf8());
    geojsonLayer->load(geojsonDocument);

    // Add the GeoJSON layer
    GraphicsOverlay* geoJsonPointsOverlay = geojsonLayer->pointsOverlay();
    m_mapView->graphicsOverlays()->append(geoJsonPointsOverlay);
    GraphicsOverlay* geoJsonLinesOverlay = geojsonLayer->linesOverlay();
    m_mapView->graphicsOverlays()->append(geoJsonLinesOverlay);
    GraphicsOverlay* geoJsonAreasOverlay = geojsonLayer->areasOverlay();
    m_mapView->graphicsOverlays()->append(geoJsonAreasOverlay);
    return true;
}

bool MapViewModel::addGeoJsonPointFeatures(const QString& features, const QString& renderer)
{
    SimpleGeoJsonLayer* geojsonLayer = new SimpleGeoJsonLayer(this);
    QJsonDocument geojsonDocument = QJsonDocument::fromJson(features.toUtf8());
    geojsonLayer->load(geojsonDocument);

    // Add the GeoJSON layer
    GraphicsOverlay* geoJsonPointsOverlay = geojsonLayer->pointsOverlay();
    Renderer* geoJsonRenderer = Renderer::fromJson(renderer, this);
    geoJsonPointsOverlay->setRenderer(geoJsonRenderer);
    m_mapView->graphicsOverlays()->append(geoJsonPointsOverlay);
    return true;
}

bool MapViewModel::addGeoJsonLineFeatures(const QString& features, const QString& renderer)
{
    SimpleGeoJsonLayer* geojsonLayer = new SimpleGeoJsonLayer(this);
    QJsonDocument geojsonDocument = QJsonDocument::fromJson(features.toUtf8());
    geojsonLayer->load(geojsonDocument);

    // Add the GeoJSON layer
    GraphicsOverlay* geoJsonLinesOverlay = geojsonLayer->linesOverlay();
    Renderer* geoJsonRenderer = Renderer::fromJson(renderer, this);
    geoJsonLinesOverlay->setRenderer(geoJsonRenderer);
    m_mapView->graphicsOverlays()->append(geoJsonLinesOverlay);
    return true;
}

bool MapViewModel::addGeoJsonPolygonFeatures(const QString& features, const QString& renderer)
{
    SimpleGeoJsonLayer* geojsonLayer = new SimpleGeoJsonLayer(this);
    QJsonDocument geojsonDocument = QJsonDocument::fromJson(features.toUtf8());
    geojsonLayer->load(geojsonDocument);

    // Add the GeoJSON layer
    GraphicsOverlay* geoJsonAreasOverlay = geojsonLayer->areasOverlay();
    Renderer* geoJsonRenderer = Renderer::fromJson(renderer, this);
    geoJsonAreasOverlay->setRenderer(geoJsonRenderer);
    m_mapView->graphicsOverlays()->append(geoJsonAreasOverlay);

    /*
    GraphicListModel* graphics = geoJsonAreasOverlay->graphics();
    QList<GeoElement*> graphicsList;
    graphicsList.reserve(graphics->size());
    std::for_each(graphics->begin(), graphics->end(), [&graphicsList](Graphic* graphic)
    {
        graphicsList.append(static_cast<GeoElement*>(graphic));
    });
    FeatureCollectionTable* geojsonFeatureCollectionTable = new FeatureCollectionTable(graphicsList, QList<Field>(), this);
    geojsonFeatureCollectionTable->setRenderer(geoJsonRenderer);
    FeatureCollection* geojsonFeatureCollection = new FeatureCollection(this);
    geojsonFeatureCollection->tables()->append(geojsonFeatureCollectionTable);
    FeatureCollectionLayer* geojsonFeatureCollectionLayer = new FeatureCollectionLayer(geojsonFeatureCollection, this);
    m_map->operationalLayers()->append(geojsonFeatureCollectionLayer);
    */
    return true;
}

void MapViewModel::onMouseClicked(QMouseEvent& mouseEvent)
{
    if (!m_mapView)
    {
        return;
    }

    // Emit the clicked location
    QPointF screenPosition = mouseEvent.position();
    Point mapClickLocation = m_mapView->screenToLocation(screenPosition.x(), screenPosition.y());
    emit mapViewClicked(mapClickLocation.toJson());
}
