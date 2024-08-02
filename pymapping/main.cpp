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

#include <iostream>

using namespace std;

#include <pybind11/pybind11.h>
#include <pybind11/chrono.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>

#include <ArcGISRuntimeEnvironment.h>
#include <MapQuickView.h>
#include <MapTypes.h>
#include <Point.h>

#include <QDir>
#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QProcessEnvironment>

#include "MapViewModel.h"

namespace py = pybind11;

using namespace Esri::ArcGISRuntime;
using namespace std;

static void initializeLocationServicesFromEnvironment()
{
    QString apiKeyName = "arcgis_api_key";
    QProcessEnvironment systemEnvironment = QProcessEnvironment::systemEnvironment();
    if (systemEnvironment.contains(apiKeyName))
    {
        QString apiKey = systemEnvironment.value(apiKeyName);
        if (apiKey.isEmpty())
        {
            qWarning() << "Use of Esri location services, including basemaps, requires" <<
                "you to authenticate with an ArcGIS identity or set the API Key property.";
            return;
        }

        ArcGISRuntimeEnvironment::setApiKey(apiKey);
        qDebug() << "API key from the current environment was used to authenticate against the ArcGIS Location Platform services.";
    }
    else
    {
        qWarning() << "Use of Esri location services, including basemaps, requires" <<
            "you to authenticate with an ArcGIS identity or set the API Key property.";
    }
}

static void initialize(const string& apiKey = "")
{
    // Use of Esri location services, including basemaps and geocoding, requires
    // either an ArcGIS identity or an API key. For more information see
    // https://links.esri.com/arcgis-runtime-security-auth.

    // 1. ArcGIS identity: An ArcGIS named user account that is a member of an
    // organization in ArcGIS Online or ArcGIS Enterprise.

    // 2. API key: A permanent key that gives your application access to Esri
    // location services. Create a new API key or access existing API keys from
    // your ArcGIS for Developers dashboard (https://links.esri.com/arcgis-api-keys).
    if (apiKey.empty())
    {
        initializeLocationServicesFromEnvironment();
    }
    else
    {
        ArcGISRuntimeEnvironment::setApiKey(QString::fromStdString(apiKey));
    }

    // Production deployment of applications built with ArcGIS Maps SDK requires you to
    // license ArcGIS Maps SDK functionality. For more information see
    // https://links.esri.com/arcgis-runtime-license-and-deploy.

    // ArcGISRuntimeEnvironment::setLicense("Place license string in here");

    // Register the map view for QML
    qmlRegisterType<MapQuickView>("Esri.Mapping", 1, 0, "MapView");

    // Register the basemap style for QML
    //qmlRegisterType<BasemapStyle>("Esri.Mapping", 1, 0, "BasemapStyle");

    // Register the MapViewModel (QQuickItem) for QML
    qmlRegisterType<MapViewModel>("Esri.Mapping", 1, 0, "MapViewModel");

    // Register the core types
    //qmlRegisterType<Point>("Esri.ArcGISRuntime", 1, 0, "Point");
}

PYBIND11_MODULE(coremapping, m) {
    m.doc() = "Offers access to ArcGIS Runtime Core mapping capabilities."; // optional module docstring

    m.def("initialize", &initialize, "Initializes the underlying ArcGIS Runtime core environment.",
          py::arg("apiKey") = py::none());

    py::enum_<BasemapStyle>(m, "BasemapStyle", py::arithmetic())
        .value("ArcGISImagery", BasemapStyle::ArcGISImagery);    

    py::class_<MapViewModel>(m, "MapViewModel")
        .def("addGeoJsonFeatures", &MapViewModel::addGeoJsonFeatures, py::arg("features"))
        .def("addGeoJsonPointFeatures", &MapViewModel::addGeoJsonPointFeatures, py::arg("features"), py::arg("renderer"))
        .def("addGeoJsonLineFeatures", &MapViewModel::addGeoJsonLineFeatures, py::arg("features"), py::arg("renderer"))
        .def("addGeoJsonPolygonFeatures", &MapViewModel::addGeoJsonPolygonFeatures, py::arg("features"), py::arg("renderer"))
        .def("addFeatureLayer", &MapViewModel::addFeatureLayer, py::arg("featureServiceUrl"))
        .def("addFeatureLayerFromMobile", &MapViewModel::addFeatureLayerFromMobile, py::arg("workspacePath"), py::arg("featureClassName"))
        .def("addFeatureLayerFromGeoPackage", &MapViewModel::addFeatureLayerFromGeoPackage, py::arg("workspacePath"), py::arg("featureClassName"))
        .def("addRasterLayer", &MapViewModel::addRasterLayer, py::arg("rasterFilePath"), py::arg("opacity"))
        .def("addRasterLayerFromGeoPackage", &MapViewModel::addRasterLayerFromGeoPackage, py::arg("workspacePath"), py::arg("rasterName"), py::arg("opacity"))
        .def("clearGraphicOverlays", &MapViewModel::clearGraphicOverlays)
        .def("clearOperationalLayers", &MapViewModel::clearOperationalLayers)
        .def("loadBasemapFromTilePackage", &MapViewModel::loadBasemapFromTilePackage, py::arg("tilePackageFilePath"));
}
