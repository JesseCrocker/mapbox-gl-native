#include <mbgl/style/sources/custom_vector_source.hpp>
#include <mbgl/style/sources/custom_vector_source_impl.hpp>

namespace mbgl {
namespace style {

CustomVectorSource::CustomVectorSource(const std::string& id,
                                       const GeoJSONOptions& options,
                                       std::function<void(const CanonicalTileID&)> fetchTile)
    : Source(makeMutable<Impl>(std::move(id), options, fetchTile)) {
}

CustomVectorSource::~CustomVectorSource() = default;
  
const CustomVectorSource::Impl& CustomVectorSource::impl() const {
    return static_cast<const Impl&>(*baseImpl);
}

void CustomVectorSource::setTileData(const CanonicalTileID& tileId,
                                     const mapbox::geojson::geojson& geoJSON) {
    impl().setTileData(tileId, geoJSON);
}

void CustomVectorSource::reloadRegion(mbgl::LatLngBounds bounds, uint8_t z) {
    impl().reloadRegion(bounds, z);
}

void CustomVectorSource::reloadTile(const CanonicalTileID& tileId) {
    impl().reloadTile(tileId);
}

void CustomVectorSource::reload() {
    impl().reload();
}
  
} // namespace style
} // namespace mbgl
