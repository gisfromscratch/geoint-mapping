// GEOINT Monitor is a sample native desktop application for geospatial intelligence workflows.
// Copyright (C) 2020 Jan Tschada (gisfromscratch@live.de)
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
#include "SimpleGeoJsonLayer.h"

#include "GraphicsFactory.h"

#include <GraphicsOverlay.h>
#include <SimpleFillSymbol.h>
#include <SimpleLineSymbol.h>
#include <SimpleMarkerSymbol.h>
#include <SimpleRenderer.h>
#include <SymbolTypes.h>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

using namespace Esri::ArcGISRuntime;

SimpleGeoJsonLayer::SimpleGeoJsonLayer(QObject *parent) :
    QObject(parent),
    m_pointsOverlay(new GraphicsOverlay(this)),
    m_linesOverlay(new GraphicsOverlay(this)),
    m_areasOverlay(new GraphicsOverlay(this)),
    m_graphicsFactor(new GraphicsFactory(this))
{
    SimpleRenderer* fillRenderer = new SimpleRenderer(this);
    SimpleFillSymbol* fillSymbol = new SimpleFillSymbol(SimpleFillSymbolStyle::Solid, QColor("#d3c2a6"), this);
    fillSymbol->setOutline(new SimpleLineSymbol(SimpleLineSymbolStyle::Solid, Qt::black, 4, this));
    fillRenderer->setSymbol(fillSymbol);
    m_areasOverlay->setRenderer(fillRenderer);
    m_areasOverlay->setOpacity(0.35f);

    SimpleRenderer* lineRenderer = new SimpleRenderer(this);
    SimpleLineSymbol* lineSymbol = new SimpleLineSymbol(SimpleLineSymbolStyle::Solid, Qt::black, 5, this);
    lineRenderer->setSymbol(lineSymbol);
    m_linesOverlay->setRenderer(lineRenderer);
    m_linesOverlay->setOpacity(0.35f);

    SimpleRenderer* markerRenderer = new SimpleRenderer(this);
    SimpleMarkerSymbol* markerSymbol = new SimpleMarkerSymbol(SimpleMarkerSymbolStyle::Circle, QColor("#d3c2a6"), 12, this);
    markerSymbol->setOutline(new SimpleLineSymbol(SimpleLineSymbolStyle::Solid, Qt::black, 4, this));
    markerRenderer->setSymbol(markerSymbol);
    m_pointsOverlay->setRenderer(markerRenderer);
}

GraphicsOverlay* SimpleGeoJsonLayer::pointsOverlay() const
{
    return m_pointsOverlay;
}

GraphicsOverlay* SimpleGeoJsonLayer::linesOverlay() const
{
    return m_linesOverlay;
}

GraphicsOverlay* SimpleGeoJsonLayer::areasOverlay() const
{
    return m_areasOverlay;
}

void SimpleGeoJsonLayer::load(const QJsonDocument &geoJsonDocument)
{
    if (geoJsonDocument.isNull())
    {
        qDebug() << "JSON is invalid!";
        return;
    }
    if (!geoJsonDocument.isObject())
    {
        qDebug() << "JSON document is not an object!";
        return;
    }

    QJsonObject geoJsonObject = geoJsonDocument.object();
    QJsonArray geoJsonFeaturesArray = geoJsonObject["features"].toArray();
    if (!m_graphicsFactor->createGraphics(geoJsonFeaturesArray, m_pointsOverlay, m_linesOverlay, m_areasOverlay))
    {
        qDebug() << "No GeoJSON feature was added!";
    }
}
