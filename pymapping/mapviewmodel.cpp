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

#include "mapviewmodel.h"

#include <Basemap.h>
#include <Map.h>
#include <MapTypes.h>
#include <MapQuickView.h>

using namespace Esri::ArcGISRuntime;

MapViewModel::MapViewModel(QObject* parent /* = nullptr */):
    QObject(parent),
    m_map(new Map(BasemapStyle::ArcGISStreets, this))
{
    // Instantiate
    qDebug() << "Map view model was instantiated.";
}

MapViewModel::~MapViewModel()
{
}

MapQuickView* MapViewModel::mapView() const
{
    return m_mapView;
}

// Set the view (created in QML)
void MapViewModel::setMapView(MapQuickView* mapView)
{
    if (!mapView || mapView == m_mapView)
    {
        return;
    }

    m_mapView = mapView;
    m_mapView->setMap(m_map);

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
