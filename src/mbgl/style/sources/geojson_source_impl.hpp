#pragma once

#include <mbgl/style/source_impl.hpp>
#include <mbgl/style/sources/geojson_source.hpp>
#include <mbgl/util/range.hpp>

#include <mapbox/geojsonvt.hpp>
#include <supercluster.hpp>

namespace mbgl {

class AsyncRequest;
class CanonicalTileID;

namespace style {

class GeoJSONData {
public:
    virtual ~GeoJSONData() = default;
    virtual mapbox::geometry::feature_collection<int16_t> getTile(const CanonicalTileID&) = 0;
};

class GeoJSONVTData : public GeoJSONData {
public:
    GeoJSONVTData(const GeoJSON& geoJSON,
                  const mapbox::geojsonvt::Options& options);
    
    mapbox::geometry::feature_collection<int16_t> getTile(const CanonicalTileID& tileID) final;
    
private:
    mapbox::geojsonvt::GeoJSONVT impl;
};
  
class SuperclusterData : public GeoJSONData {
public:
    SuperclusterData(const mapbox::geometry::feature_collection<double>& features,
                     const mapbox::supercluster::Options& options);
    
    mapbox::geometry::feature_collection<int16_t> getTile(const CanonicalTileID& tileID) final;
    
private:
    mapbox::supercluster::Supercluster impl;
};
  
class GeoJSONSource::Impl : public Source::Impl {
public:
    Impl(std::string id, GeoJSONOptions);
    Impl(const GeoJSONSource::Impl&, const GeoJSON&);
    ~Impl() final;

    Range<uint8_t> getZoomRange() const;
    GeoJSONData* getData() const;

    optional<std::string> getAttribution() const final;

private:
    GeoJSONOptions options;
    std::unique_ptr<GeoJSONData> data;
};

} // namespace style
} // namespace mbgl
