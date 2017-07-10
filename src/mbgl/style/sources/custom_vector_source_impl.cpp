#include <mbgl/style/sources/custom_vector_source_impl.hpp>

#include <mbgl/style/source_observer.hpp>
#include <mbgl/tile/geojson_tile.hpp>
#include <mbgl/tile/vector_tile.hpp>
#include <mbgl/util/tile_cover.hpp>
#include <mbgl/renderer/tile_parameters.hpp>

#include <mapbox/geojsonvt.hpp>
#include <supercluster.hpp>

namespace mbgl {
namespace style {

CustomVectorSource::Impl::Impl(std::string id_,
                               const GeoJSONOptions options_,
                               FetchTileFunction fetchTile_)
    : Source::Impl(SourceType::Computed, std::move(id_)),
    fetchTile(fetchTile_) ,
    options(options_) {
}

CustomVectorSource::Impl::~Impl() = default;

Range<uint8_t> CustomVectorSource::Impl::getZoomRange() const {
    return { options.minzoom, options.maxzoom };
}

uint16_t CustomVectorSource::Impl::getTileSize() const {
    return options.tileSize;
}

void CustomVectorSource::Impl::reload() {
    reloadRequested = true;
}

bool CustomVectorSource::Impl::needsReload() const {
    return reloadRequested;
}
  
/*
void CustomVectorSource::Impl::reloadTile(const CanonicalTileID& tileId) {
    if (cache.has(OverscaledTileID(tileId.z, tileId.x, tileId.y))) {
        cache.clear();
    }
    for (auto const& item : tiles) {
        GeoJSONTile* tile = static_cast<GeoJSONTile*>(item.second.get());
        if (tile->id.canonical == tileId) {
            fetchTile(tileId);
        }
    }
}

void CustomVectorSource::Impl::reloadRegion(mbgl::LatLngBounds bounds, const uint8_t z) {
    for (const auto& tile : mbgl::util::tileCover(bounds, z)) {
        reloadTile(tile.canonical);
    }
}
*/

optional<std::string> CustomVectorSource::Impl::getAttribution() const {
    return {};
}

} // namespace style
} // namespace mbgl
