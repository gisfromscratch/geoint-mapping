from abc import ABC
from typing import List, Protocol


class GeoElementsOverlayModel(ABC):
    """
    Model instance managing geoelements with graphic overlays.
    """

    def getCount(self) -> int:
        """
        Returns the number of graphic overlays of this overlay model.
        """

    def toDict(self, index: int) -> List[dict]:
        """
        Returns an array of JSON representation of all geoelements from a graphics overlay.

        :param index: The index of the graphics overlay.
        """


class MapViewModel(ABC):
    """
    Model instance managing a map view component.
    """

    def updateBasemapStyle(self, basemapStyle: str) -> None:
        """
        Updates the basemap style of this map view model.

        :param basemapStyle: The new basemap style like OsmStandard, ArcGISImagery...
        """

    def loadBasemapFromTilePackage(self, tilePackageFilePath: str) -> None:
        """
        Loads a basemap using a local tile cache (.tpk/.tpkx).
        Represents a cache of rendered map tiles that can be used to create a layer.

        :param tilePackageFilePath: The file path to the tile package (.tpk/.tpkx).
        """

    def loadBasemapFromVectorTilePackage(self, vectorTilePackageFilePath: str) -> None:
        """
        Loads a basemap using a local vector tile cache (.vtpk/.vtpkx).
        Represents a cache of rendered map tiles that can be used to create a layer.

        :param vectorTilePackageFilePath: The file path to the tile package (.vtpk/.vtpkx).
        """

    def loadBasemapFromWMTS(self, wmtsServiceUrl: str, layerIndex: int) -> None:
        """
        Loads a basemap using an Open Geospatial Consortium (OGC) Web Map Tile Service (WMTS) endpoint.
        A WMTS service provides access to a set of cached tiles at predefined scales.
        The service provides a collection of tile matrix sets, each of these is composed of one or more tile matrices for a series of fixed scales.
        As the scale decreases, the number of tiles in the matrix increases - each tile being of a smaller geographic area and of a higher resolution.
        The metadata about an individual Web Map Tile Service (WMTS) describes which layers are offered through a WMTS endpoint.
        Each layer can be accessed using a layer index.

        :param wmtsServiceUrl: URL of the OGC WMTS endpoint.
        :param layerIndex: The index of the WMTS layer.
        """

    def addGeoJsonFeatures(self, features: str) -> None:
        """
        Adds the GeoJSON features into a graphics collection of this map view model.

        :param features: The GeoJSON representation of the features.
        """

    def addGeoJsonPointFeatures(self, features: str, renderer: str) -> None:
        """
        Adds the GeoJSON point features into a graphics collection of this map view model.

        :param features: The GeoJSON representation of the features.
        :param renderer: The JSON representation of the renderer.
        """

    def addGeoJsonLineFeatures(self, features: str, renderer: str) -> None:
        """
        Adds the GeoJSON line features into a graphics collection of this map view model.

        :param features: The GeoJSON representation of the features.
        :param renderer: The JSON representation of the renderer.
        """

    def addGeoJsonPolygonFeatures(self, features: str, renderer: str) -> None:
        """
        Adds the GeoJSON polyline features into a graphics collection of this map view model.

        :param features: The GeoJSON representation of the features.
        :param renderer: The JSON representation of the renderer.
        """

    def addGeometries(self, geometries: str, renderer: str) -> None:
        """
        Adds the Esri JSON geometries into a feature collection of this map view model.

        :param geometries: The Esri JSON representation of the features.
        :param renderer: The JSON representation of the renderer.
        """

    def addFeatureLayer(self, featureServiceUrl: str) -> None:
        """
        Adds a feature service to this map view model.

        :param featureServiceUrl: The public URL of the feature service.
        """

    def addFeatureLayerFromMobile(self, workspacePath: str, featureClassName: str="") -> None:
        """
        Adds a feature layer from a mobile map package (.mpkx) to this map view model.

        :param workspacePath: The local file path to the mobile map package (.mpkx).
        :param featureClassName: The name of the feature class. If empty all feature classes are loaded.
        """

    def addFeatureLayerFromGeoPackage(self, workspacePath: str, featureClassName: str="") -> None:
        """
        Adds a feature layer from a geopackage (.gpkg) to this map view model.

        :param workspacePath: The local file path to the geopackage (.gpkg).
        :param featureClassName: The name of the feature class. If empty all feature classes are loaded.
        """

    def addRasterLayer(self, rasterFilePath: str, opacity: float=0.7) -> None:
        """
        Adds a local raster file to this map view model.

        :param rasterFilePath: The local file path to the raster file.
        :param opacity: The opacity of this layer as a float value between 0.0 (fully transparent) and 1.0 (fully opaque).
        """

    def addRasterLayerFromGeoPackage(self, workspacePath: str, rasterName: str, opacity: float=0.7) -> None:
        """
        Adds a raster from a geopackage (.gpkg) to this map view model.

        :param workspacePath: The local file path to the geopackage (.gpkg).
        :param rasterName: The name of the raster. If empty all rasters are loaded.
        :param opacity: The opacity of this layer as a float value between 0.0 (fully transparent) and 1.0 (fully opaque).
        """

    def clearGraphicOverlays(self) -> None:
        """
        Removes all graphic overlays from this map view model.
        """

    def clearOperationalLayers(self) -> None:
        """
        Removes all operational layers from this map view model.
        """
