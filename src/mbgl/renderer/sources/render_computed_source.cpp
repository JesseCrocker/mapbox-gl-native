#include <mbgl/algorithm/generate_clip_ids.hpp>
#include <mbgl/algorithm/generate_clip_ids_impl.hpp>
#include <mbgl/renderer/sources/render_computed_source.hpp>
#include <mbgl/renderer/render_tile.hpp>
#include <mbgl/renderer/painter.hpp>
#include <mbgl/style/sources/geojson_source_impl.hpp>
#include <mbgl/tile/geojson_tile.hpp>

#include <mapbox/geojsonvt.hpp>
#include <supercluster.hpp>

namespace mbgl {

using namespace style;

RenderComputedSource::RenderComputedSource(Immutable<style::CustomVectorSource::Impl> impl_)
    : RenderSource(impl_) {
    tilePyramid.setObserver(this);
}

const style::CustomVectorSource::Impl& RenderComputedSource::impl() const {
    return static_cast<const style::CustomVectorSource::Impl&>(*baseImpl);
}

bool RenderComputedSource::isLoaded() const {
    return tilePyramid.isLoaded();
}

void RenderComputedSource::update(Immutable<style::Source::Impl> baseImpl_,
                                 const std::vector<Immutable<Layer::Impl>>& layers,
                                 const bool needsRendering,
                                 const bool needsRelayout,
                                 const TileParameters& parameters) {
    std::swap(baseImpl, baseImpl_);

    enabled = needsRendering;
    if (impl().needsReload() ||
        (fetchTile.target_type() != impl().fetchTile.target_type() ||
        fetchTile.target<void>() != impl().fetchTile.target<void>())) { //Not sure if this check is right
        const_cast<style::CustomVectorSource::Impl&>(impl()).setReloaded();
        fetchTile = impl().fetchTile;
        
        // TODO: this removes existing buckets, and will cause flickering.
        // Should instead refresh tile data in place.
        tilePyramid.tiles.clear();
        tilePyramid.renderTiles.clear();
        tilePyramid.cache.clear();
    }
    
    if(!fetchTile) {
        return;
    }
    
    FetchTileCallback callback = [this](const CanonicalTileID t, const mapbox::geojson::geojson geoJSON){
        setTileData(t, geoJSON);
    };
    tilePyramid.update(layers,
                       needsRendering,
                       needsRelayout,
                       parameters,
                       SourceType::GeoJSON,
                       util::tileSize,
                       impl().getZoomRange(),
                       [&] (const OverscaledTileID& tileID) {
                           fetchTile(tileID.canonical, callback);
                           return std::make_unique<GeoJSONTile>(tileID, impl().id, parameters);
                       });
}

void RenderComputedSource::setTileData(const CanonicalTileID& tileID,
                                       const mapbox::geojson::geojson& geoJSON) const {
    if (geoJSON.is<FeatureCollection>() && geoJSON.get<FeatureCollection>().empty()) {
        for (auto const& item : tilePyramid.tiles) {
            GeoJSONTile* tile = static_cast<GeoJSONTile*>(item.second.get());
            if (tile->id.canonical == tileID) {
                tile->updateData(mapbox::geometry::feature_collection<int16_t>());
            }
        }
    } else {
        constexpr double scale = util::EXTENT / util::tileSize;
        std::unique_ptr<GeoJSONData> data;
        if (impl().options.cluster
            && geoJSON.is<mapbox::geometry::feature_collection<double>>()
            && !geoJSON.get<mapbox::geometry::feature_collection<double>>().empty()) {
            mapbox::supercluster::Options clusterOptions;
            clusterOptions.maxZoom = impl().options.clusterMaxZoom;
            clusterOptions.extent = util::EXTENT;
            clusterOptions.radius = std::round(scale * impl().options.clusterRadius);
            data = std::make_unique<SuperclusterData>(
                                                      geoJSON.get<mapbox::geometry::feature_collection<double>>(), clusterOptions);
        } else {
            mapbox::geojsonvt::Options vtOptions;
            vtOptions.maxZoom = impl().options.maxzoom;
            vtOptions.extent = util::EXTENT;
            vtOptions.buffer = std::round(scale * impl().options.buffer);
            vtOptions.tolerance = scale * impl().options.tolerance;
            data = std::make_unique<GeoJSONVTData>(geoJSON, vtOptions);
        }
        
        for (auto const& item : tilePyramid.tiles) {
            GeoJSONTile* tile = static_cast<GeoJSONTile*>(item.second.get());
            if (tile->id.canonical == tileID) {
                tile->updateData(data->getTile(tile->id.canonical));
            }
        }
    }
}

void RenderComputedSource::startRender(Painter& painter) {
    painter.clipIDGenerator.update(tilePyramid.getRenderTiles());
    tilePyramid.startRender(painter);
}

void RenderComputedSource::finishRender(Painter& painter) {
    tilePyramid.finishRender(painter);
}

std::vector<std::reference_wrapper<RenderTile>> RenderComputedSource::getRenderTiles() {
    return tilePyramid.getRenderTiles();
}

std::unordered_map<std::string, std::vector<Feature>>
RenderComputedSource::queryRenderedFeatures(const ScreenLineString& geometry,
                                           const TransformState& transformState,
                                           const RenderStyle& style,
                                           const RenderedQueryOptions& options) const {
    return tilePyramid.queryRenderedFeatures(geometry, transformState, style, options);
}

std::vector<Feature> RenderComputedSource::querySourceFeatures(const SourceQueryOptions& options) const {
    return tilePyramid.querySourceFeatures(options);
}

void RenderComputedSource::onLowMemory() {
    tilePyramid.onLowMemory();
}

void RenderComputedSource::dumpDebugLogs() const {
    tilePyramid.dumpDebugLogs();
}

} // namespace mbgl
