#import <Foundation/Foundation.h>

#import "MGLFoundation.h"
#import "MGLOfflineRegion.h"
#import "MGLGeometry.h"

NS_ASSUME_NONNULL_BEGIN

/**
 An offline region defined by a style URL, geographic coordinate bounds, and
 range of zoom levels.
 
 To minimize the resources required by an irregularly shaped offline region,
 use the MGLShapeOfflineRegion class instead.
 
 #### Related examples
 See the <a href="https://www.mapbox.com/ios-sdk/maps/examples/offline-pack/">
 Download an offline map</a> example to learn how to define an offline region
 to be downloaded to a user's device.
 */
MGL_EXPORT
@interface MGLTileListOfflineRegion : NSObject <MGLOfflineRegion, NSSecureCoding, NSCopying>

/**
 The coordinate bounds for the geographic region covered by the downloaded
 tiles.
 */
@property (nonatomic, readonly) MGLCoordinateBounds bounds;

/**
 The minimum zoom level for which to download tiles and other resources.

 For more information about zoom levels, `-[MGLMapView zoomLevel]`.
 */
@property (nonatomic, readonly) double minimumZoomLevel;

/**
 The maximum zoom level for which to download tiles and other resources.

 For more information about zoom levels, `-[MGLMapView zoomLevel]`.
 */
@property (nonatomic, readonly) double maximumZoomLevel;

/**
 A specific list of tileIds to download for this region
 */
@property (nonatomic, readonly, nullable) NSArray *tileList;

- (instancetype)init NS_UNAVAILABLE;

/**
 Initializes a newly created offline region with the given style URL, geographic
 coordinate bounds, and range of zoom levels.

 This is the designated initializer for `MGLTilePyramidOfflineRegion`.

 @param styleURL URL of the map style for which to download resources. The URL
    may be a full HTTP or HTTPS URL or a Mapbox URL indicating the style’s map
    ID (`mapbox://styles/{user}/{style}`). Specify `nil` for the default style.
    Relative file URLs cannot be used as offline style URLs. To download the
    online resources required by a local style, specify a URL to an online copy
    of the style.
 @param bounds The coordinate bounds for the geographic region to be covered by
    the downloaded tiles.
 @param minimumZoomLevel The minimum zoom level to be covered by the downloaded
    tiles. This parameter should be set to at least 0 but no greater than the
    value of the `maximumZoomLevel` parameter. For each required tile source, if
    this parameter is set to a value less than the tile source’s minimum zoom
    level, the download covers zoom levels down to the tile source’s minimum
    zoom level.
 @param maximumZoomLevel The maximum zoom level to be covered by the downloaded
    tiles. This parameter should be set to at least the value of the
    `minimumZoomLevel` parameter. For each required tile source, if this
    parameter is set to a value greater than the tile source’s minimum zoom
    level, the download covers zoom levels up to the tile source’s maximum zoom
    level.
 @params tileList list of tiles keys as NSNumbers. See MGLTileID.h MGLTileIDMake MGLTileKey
 */
- (instancetype)initWithStyleURL:(NSURL *)styleURL bounds:(MGLCoordinateBounds)bounds fromZoomLevel:(double)minimumZoomLevel toZoomLevel:(double)maximumZoomLevel tileList:(nullable NSArray <NSNumber *> *)tileList;

@end

NS_ASSUME_NONNULL_END
