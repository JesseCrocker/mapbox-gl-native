#pragma once

#include <mbgl/renderer/render_source.hpp>
#include <mbgl/renderer/tile_pyramid.hpp>
#include <mbgl/style/sources/custom_vector_source.hpp>
#include <mbgl/style/sources/custom_vector_source_impl.hpp>

namespace mbgl {

class RenderComputedSource : public RenderSource {
public:
    RenderComputedSource(Immutable<style::CustomVectorSource::Impl>);

    bool isLoaded() const final;

    void update(Immutable<style::Source::Impl>,
                const std::vector<Immutable<style::Layer::Impl>>&,
                bool needsRendering,
                bool needsRelayout,
                const TileParameters&) final;
  
    void startRender(Painter&) final;
    void finishRender(Painter&) final;

    std::vector<std::reference_wrapper<RenderTile>> getRenderTiles() final;

    std::unordered_map<std::string, std::vector<Feature>>
    queryRenderedFeatures(const ScreenLineString& geometry,
                          const TransformState& transformState,
                          const RenderStyle& style,
                          const RenderedQueryOptions& options) const final;

    std::vector<Feature>
    querySourceFeatures(const SourceQueryOptions&) const final;

    void onLowMemory() final;
    void dumpDebugLogs() const final;

private:
    const style::CustomVectorSource::Impl& impl() const;
    style::FetchTileFunction fetchTile;
    void setTileData(const CanonicalTileID& tileID,
                     const mapbox::geojson::geojson& geoJSON) const;
  
    TilePyramid tilePyramid;
};

template <>
inline bool RenderSource::is<RenderComputedSource>() const {
    return baseImpl->type == SourceType::Computed;
}

} // namespace mbgl
