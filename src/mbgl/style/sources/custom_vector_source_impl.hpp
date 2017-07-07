#pragma once

#include <mbgl/style/source_impl.hpp>
#include <mbgl/style/sources/custom_vector_source.hpp>
#include <mbgl/util/range.hpp>
#include <mbgl/tile/tile.hpp>
#include <mbgl/renderer/update_parameters.hpp>

namespace mbgl {
namespace style {

class CustomVectorSource::Impl : public Source::Impl {
public:
    Impl(std::string id,
         GeoJSONOptions options,
         std::function<void(const CanonicalTileID&)> fetchTile);
    ~Impl() final;
  
    void setTileData(const CanonicalTileID& tileID, const mapbox::geojson::geojson&) const;
    void reloadTile(const CanonicalTileID& tileID) const;
    void reloadRegion(mbgl::LatLngBounds bounds, uint8_t z) const;
    void reload() const;

    optional<std::string> getAttribution() const final;

private:
    uint16_t getTileSize() const;
    Range<uint8_t> getZoomRange() const;
//    std::unique_ptr<Tile> createTile(const OverscaledTileID&, const UpdateParameters&);
    GeoJSONOptions options;
    std::function<void(const CanonicalTileID&)> fetchTile;
};

} // namespace style
} // namespace mbgl
