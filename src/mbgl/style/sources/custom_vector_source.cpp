#include <mbgl/style/sources/custom_vector_source.hpp>
#include <mbgl/style/sources/custom_vector_source_impl.hpp>
#include <mbgl/style/source_observer.hpp>

namespace mbgl {
namespace style {

  
CustomVectorSource::CustomVectorSource(const std::string& id,
                                       const GeoJSONOptions& options,
                                       FetchTileFunction fetchTile)
    : Source(makeMutable<Impl>(std::move(id), options, fetchTile)) {
}

CustomVectorSource::~CustomVectorSource() = default;
  
const CustomVectorSource::Impl& CustomVectorSource::impl() const {
    return static_cast<const Impl&>(*baseImpl);
}

/*
void CustomVectorSource::reloadRegion(mbgl::LatLngBounds bounds, uint8_t z) {
  auto t = impl();
  t.reloadRegion(bounds, z);
}

void CustomVectorSource::reloadTile(const CanonicalTileID& tileId) {
    auto t = impl();
    t.reloadTile(tileId);
}
*/
 
void CustomVectorSource::reload() {
    auto t = impl();
    t.reload();
    observer->onSourceChanged(*this);
}
  
} // namespace style
} // namespace mbgl
