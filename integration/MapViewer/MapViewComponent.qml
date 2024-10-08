import QtQuick
import QtQuick.Controls
import Esri.Mapping

import esri.mapping.slots

Item {

    // Create MapQuickView here, and create its Map etc. in C++ code
    MapView {
        id: view
        anchors.fill: parent
        // set focus to enable keyboard navigation
        focus: true
    }

    Bridge {
        id: bridge
    }

    GeoElementsInspector {
        id: inspector
    }

    // Declare the C++ instance which creates the map etc. and supply the view
    MapViewModel {
        id: model
        mapView: view
        basemapStyle: "OsmStandard"
        
        onMapViewChanged: {
            console.log(bridge.getValue("Map view changed"));
            console.log(model.overlayModel.count);
            //model.startSketching(MapViewModel.PolygonSketchMode);
        }

        /*
        onMapViewExtentChanged: {
            console.log(model.mapViewExtent);
        }

        onMapViewCenterChanged: {
            console.log(model.mapViewCenter);
        }        
        
        onSketchCompleted: geometry => {
            console.log(geometry);
            var geometries = [JSON.parse(geometry)];
            var renderer = {
                'label': '',
                'description': '',
                'type': 'simple',
                'symbol': {'type': 'esriSFS',
                'style': 'esriSFSSolid',
                'color': [0, 128, 0, 128],
                'outline': {'type': 'esriSLS',
                'style': 'esriSLSSolid',
                'color': [110, 110, 110, 255],
                'width': 1}}}
            model.addGeometries(JSON.stringify(geometries), JSON.stringify(renderer));
        }
        */

        onMapViewClicked: location => {
            console.log(location);
            
            // Access the GeoElementsOverlayModel
            if (0 < model.overlayModel.count) {
                var geoelements = model.overlayModel.toDict(0);
                for (var index=0; index<geoelements.length; index++) {
                    var geoelement = geoelements[index];
                    for (var attributeKey in geoelement) {
                        console.log(attributeKey, ":", geoelement[attributeKey]);
                    }
                }
            }

            // Call Python slot using the GeoElementsOverlayModel
            inspector.inspect(model.overlayModel);
            /*
            model.clearGraphicOverlays();
            model.mapViewExtent = '{"xmin":1355768.573867436,"ymin":6764128.1390294079,"xmax":1366278.4719742704,"ymax":6772010.5626095338,"spatialReference":{"wkid":102100,"latestWkid":3857}}';

            model.addFeatureLayer("https://services9.arcgis.com/RHVPKKiFTONKtxq3/ArcGIS/rest/services/USA_Wildfires_v1/FeatureServer/0");
            model.addFeatureLayerFromGeoPackage("/mnt/data/GIS/US/Alaska/AK_Structures.gpkg", "AK_Structures");
            model.addRasterLayer("/mnt/data/GIS/Sentinel-2/x_____xUhQN_f7Y9ij8HDjj7W61Bw..x_____x_ags_b176d09a_7b38_471b_a75e_ad7f4e9dc00a.tif");
            
            model.clearOperationalLayers();
            model.loadBasemapFromTilePackage("/mnt/data/GIS/US/DSA_data/DSA/BasemapData/ImageryMonterey.tpk");
            model.loadMapFromMobilePackage("/mnt/data/GIS/US/Yellowstone.mmpk");
            model.loadBasemapFromWMTS("https://sampleserver6.arcgisonline.com/arcgis/rest/services/WorldTimeZones/MapServer/WMTS");

            model.startSketching(MapViewModel.PointSketchMode);
            */
        }
    }
}