#pragma once

#include <mbgl/style/source.hpp>
#include <mbgl/style/sources/geojson_source.hpp>
#include <mbgl/util/geo.hpp>
#include <mbgl/util/geojson.hpp>

namespace mbgl {
namespace style {

typedef std::function<void(const CanonicalTileID& tileID, const mapbox::geojson::geojson&)> FetchTileCallback;
typedef std::function<void(const CanonicalTileID&, FetchTileCallback)> FetchTileFunction;

class CustomVectorSource : public Source {
public:
    CustomVectorSource(const std::string& id,
                       const GeoJSONOptions& options,
                       FetchTileFunction fetchTile);
    ~CustomVectorSource() final;
  
    void reloadTile(const CanonicalTileID&);
    void reloadRegion(mbgl::LatLngBounds bounds, uint8_t z);
    void reload();
    void loadDescription(FileSource&) final {}

    // Private implementation

  class Impl;
  const Impl& impl() const;
};

template <>
inline bool Source::is<CustomVectorSource>() const {
    return getType() == SourceType::GeoJSON;
}

} // namespace style
} // namespace mbgl
