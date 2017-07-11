#pragma once

#include <mbgl/util/range.hpp>
#include <mbgl/style/source_impl.hpp>
#include <mbgl/style/sources/custom_vector_source.hpp>
#include <mbgl/tile/tile.hpp>

namespace mbgl {
namespace style {
  
class CustomVectorSource::Impl : public Source::Impl {
public:
    Impl(std::string id,
         GeoJSONOptions options,
         FetchTileFunction fetchTile);
    ~Impl() final;
  
    void reload();
    bool needsReload() const;
    void setReloaded();

//    void reloadTile(const CanonicalTileID& tileID);
//    void reloadRegion(mbgl::LatLngBounds bounds, uint8_t z);
    Range<uint8_t> getZoomRange() const;

    optional<std::string> getAttribution() const final;
    FetchTileFunction fetchTile;
    GeoJSONOptions options;
  

private:
    bool reloadRequested;
    uint16_t getTileSize() const;
};

} // namespace style
} // namespace mbgl
