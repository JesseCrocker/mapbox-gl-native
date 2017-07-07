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
                               std::function<void(const CanonicalTileID&)> fetchTile_)
    : Source::Impl(SourceType::Vector, std::move(id_)),
      options(options_),
      fetchTile(fetchTile_) {
}

CustomVectorSource::Impl::~Impl() = default;

Range<uint8_t> CustomVectorSource::Impl::getZoomRange() const {
    return { options.minzoom, options.maxzoom };
}

uint16_t CustomVectorSource::Impl::getTileSize() const {
    return options.tileSize;
}
  
//std::unique_ptr<Tile> CustomVectorSource::Impl::createTile(const OverscaledTileID& tileID,
//                                                           const UpdateParameters& parameters) {
//    auto tilePointer = std::make_unique<GeoJSONTile>(tileID, "", parameters);
//    fetchTile(tileID.canonical);
//    return std::move(tilePointer);
//}

void CustomVectorSource::Impl::setTileData(const CanonicalTileID& /*tileID*/,
                                           const mapbox::geojson::geojson& /*geoJSON*/) const {
/*    constexpr double scale = util::EXTENT / util::tileSize;

    if (geoJSON.is<FeatureCollection>() && geoJSON.get<FeatureCollection>().empty()) {
        for (auto const& item : tiles) {
            GeoJSONTile* tile = static_cast<GeoJSONTile*>(item.second.get());
            if (tile->id.canonical == tileID) {
                tile->updateData(mapbox::geometry::feature_collection<int16_t>());
            }
        }
    } else {
        variant<GeoJSONVTPointer, SuperclusterPointer> geoJSONOrSupercluster;
        if (!options.cluster) {
            mapbox::geojsonvt::Options vtOptions;
            vtOptions.maxZoom = options.maxzoom;
            vtOptions.extent = util::EXTENT;
            vtOptions.buffer = std::round(scale * options.buffer);
            vtOptions.tolerance = scale * options.tolerance;
            geoJSONOrSupercluster =
                std::make_unique<mapbox::geojsonvt::GeoJSONVT>(geoJSON, vtOptions);
        } else {
            mapbox::supercluster::Options clusterOptions;
            clusterOptions.maxZoom = options.clusterMaxZoom;
            clusterOptions.extent = util::EXTENT;
            clusterOptions.radius = std::round(scale * options.clusterRadius);

            const auto& features = geoJSON.get<mapbox::geometry::feature_collection<double>>();
            geoJSONOrSupercluster =
                std::make_unique<mapbox::supercluster::Supercluster>(features, clusterOptions);
        }

        for (auto const& item : tiles) {
            GeoJSONTile* tile = static_cast<GeoJSONTile*>(item.second.get());
            if (tile->id.canonical == tileID) {
                if (geoJSONOrSupercluster.is<GeoJSONVTPointer>()) {
                    tile->updateData(geoJSONOrSupercluster.get<GeoJSONVTPointer>()
                                         ->getTile(tileID.z, tileID.x, tileID.y)
                                         .features);
                } else {
                    assert(geoJSONOrSupercluster.is<SuperclusterPointer>());
                    tile->updateData(geoJSONOrSupercluster.get<SuperclusterPointer>()->getTile(
                        tileID.z, tileID.x, tileID.y));
                }
            }
        }
    }*/
}

void CustomVectorSource::Impl::reloadTile(const CanonicalTileID& /*tileId*/) const {
//    if (cache.has(OverscaledTileID(tileId.z, tileId.x, tileId.y))) {
//        cache.clear();
//    }
//    for (auto const& item : tiles) {
//        GeoJSONTile* tile = static_cast<GeoJSONTile*>(item.second.get());
//        if (tile->id.canonical == tileId) {
//            fetchTile(tileId);
//        }
//    }
}

void CustomVectorSource::Impl::reloadRegion(mbgl::LatLngBounds bounds, const uint8_t z) const {
    for (const auto& tile : mbgl::util::tileCover(bounds, z)) {
        reloadTile(tile.canonical);
    }
}

void CustomVectorSource::Impl::reload() const {
/*    cache.clear();
    for (auto const& item : tiles) {
        GeoJSONTile* tile = static_cast<GeoJSONTile*>(item.second.get());
        fetchTile(tile->id.canonical);
    }
 */
}

optional<std::string> CustomVectorSource::Impl::getAttribution() const {
    return {};
}

} // namespace style
} // namespace mbgl
