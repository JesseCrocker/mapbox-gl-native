#include <mbgl/style/sources/custom_vector_source.hpp>
#include <mbgl/style/sources/custom_vector_source_impl.hpp>

namespace mbgl {
namespace style {
    
CustomVectorSource::CustomVectorSource(std::string id, GeoJSONOptions options, std::function<void(uint8_t, uint32_t, uint32_t)> fetchTile)
      : Source(SourceType::Vector, std::make_unique<CustomVectorSource::Impl>(std::move(id), *this,  options, fetchTile)),
        impl(static_cast<Impl*>(baseImpl.get())) {
}
      
void CustomVectorSource::setTileData(uint8_t z, uint32_t x, uint32_t y, const mapbox::geojson::geojson& geoJSON) {
    if(impl != nullptr) {
        impl->setTileData(z, x, y, geoJSON);
    }
}

void CustomVectorSource::reloadRegion(mbgl::LatLngBounds bounds, uint8_t z) {
    if(impl != nullptr) {
        impl->reloadRegion(bounds, z);
    }
}
  
void CustomVectorSource::updateTile(uint8_t z, uint32_t x, uint32_t y) {
    if(impl != nullptr) {
        impl->updateTile(z, x, y);
    }
}
  
void CustomVectorSource::reload() {
    if(impl != nullptr) {
        impl->reload();
    }
}

} // namespace style
} // namespace mbgl
