#import "MGLTileListOfflineRegion.h"

#if !TARGET_OS_IPHONE && !TARGET_OS_SIMULATOR
    #import <Cocoa/Cocoa.h>
#endif

#import "MGLOfflineRegion_Private.h"
#import "MGLTileListOfflineRegion_Private.h"
#import "MGLGeometry_Private.h"
#import "MGLStyle.h"
#import "MGLLoggingConfiguration_Private.h"
#import "MGLTileID.h"
#include <mbgl/tile/tile_id.hpp>

#if TARGET_OS_IPHONE || TARGET_OS_SIMULATOR
#import "MMEConstants.h"
#endif

@interface MGLTileListOfflineRegion () <MGLOfflineRegion_Private, MGLTileListOfflineRegion_Private>

@end

@implementation MGLTileListOfflineRegion {
    NSURL *_styleURL;
}

@synthesize styleURL = _styleURL;
@synthesize includesIdeographicGlyphs = _includesIdeographicGlyphs;

- (NSDictionary *)offlineStartEventAttributes {
    return @{
             #if TARGET_OS_IPHONE || TARGET_OS_SIMULATOR
             MMEEventKeyShapeForOfflineRegion: @"tileregion",
             MMEEventKeyMinZoomLevel: @(self.minimumZoomLevel),
             MMEEventKeyMaxZoomLevel: @(self.maximumZoomLevel),
             MMEEventKeyStyleURL: self.styleURL.absoluteString ?: [NSNull null]
             #endif
             };
}

+ (BOOL)supportsSecureCoding {
    return YES;
}

- (instancetype)init {
    MGLLogInfo(@"Calling this initializer is not allowed.");
    [NSException raise:NSGenericException format:
     @"-[MGLTileListOfflineRegion init] is unavailable. "
     @"Use -initWithStyleURL:bounds:fromZoomLevel:toZoomLevel:tileList: instead."];
    return nil;
}

- (instancetype)initWithStyleURL:(NSURL *)styleURL bounds:(MGLCoordinateBounds)bounds fromZoomLevel:(double)minimumZoomLevel toZoomLevel:(double)maximumZoomLevel tileList:(nullable NSArray <NSNumber *> *)tileList {
    MGLLogDebug(@"Initializing styleURL: %@ bounds: %@ fromZoomLevel: %f toZoomLevel: %f", styleURL, MGLStringFromCoordinateBounds(bounds), minimumZoomLevel, maximumZoomLevel);
    if (self = [super init]) {
        if (!styleURL) {
            styleURL = [MGLStyle streetsStyleURLWithVersion:MGLStyleDefaultVersion];
        }

        if (!styleURL.scheme) {
            [NSException raise:MGLInvalidStyleURLException format:
             @"%@ does not support setting a relative file URL as the style URL. "
             @"To download the online resources required by this style, "
             @"specify a URL to an online copy of this style. "
             @"For Mapbox-hosted styles, use the mapbox: scheme.",
             NSStringFromClass([self class])];
        }

        _styleURL = styleURL;
        _bounds = bounds;
        _minimumZoomLevel = minimumZoomLevel;
        _maximumZoomLevel = maximumZoomLevel;
        _includesIdeographicGlyphs = YES;
        _tileList = tileList;
    }
    return self;
}

- (instancetype)initWithOfflineRegionDefinition:(const mbgl::OfflineTileListRegionDefinition &)definition {
    NSURL *styleURL = [NSURL URLWithString:@(definition.styleURL.c_str())];
    MGLCoordinateBounds bounds = MGLCoordinateBoundsFromLatLngBounds(definition.bounds);
    NSMutableArray *tileKeys = [NSMutableArray arrayWithCapacity:definition.tileList.size()];
    for(mbgl::CanonicalTileID t : definition.tileList) {
        NSNumber *tileKey = @(MGLTileKey(MGLTileIDMake(t.z, t.x, t.y)));
        [tileKeys addObject:tileKey];
    }
    return [self initWithStyleURL:styleURL bounds:bounds fromZoomLevel:definition.minZoom toZoomLevel:definition.maxZoom tileList:tileKeys];
}

- (const mbgl::OfflineRegionDefinition)offlineRegionDefinition {
#if TARGET_OS_IPHONE || TARGET_OS_SIMULATOR
    const float scaleFactor = [UIScreen instancesRespondToSelector:@selector(nativeScale)] ? [[UIScreen mainScreen] nativeScale] : [[UIScreen mainScreen] scale];
#elif TARGET_OS_MAC
    const float scaleFactor = [NSScreen mainScreen].backingScaleFactor;
#endif
    std::vector<mbgl::CanonicalTileID> tileVector;
    tileVector.reserve(_tileList.count);
    for (NSNumber * value in _tileList) {
        MGLTileID tileId = MGLTileIDFromKey([value unsignedLongLongValue]);
        mbgl::CanonicalTileID canonicalTileID = mbgl::CanonicalTileID(tileId.z, tileId.x, tileId.y);
        tileVector.push_back(canonicalTileID);
    }
    return mbgl::OfflineTileListRegionDefinition(_styleURL.absoluteString.UTF8String,
                                                 MGLLatLngBoundsFromCoordinateBounds(_bounds),
                                                 _minimumZoomLevel, _maximumZoomLevel,
                                                 scaleFactor, _includesIdeographicGlyphs, tileVector);
}

- (nullable instancetype)initWithCoder:(NSCoder *)coder {
    MGLLogInfo(@"Initializing with coder.");
    NSURL *styleURL = [coder decodeObjectForKey:@"styleURL"];
    CLLocationCoordinate2D sw = CLLocationCoordinate2DMake([coder decodeDoubleForKey:@"southWestLatitude"],
                                                           [coder decodeDoubleForKey:@"southWestLongitude"]);
    CLLocationCoordinate2D ne = CLLocationCoordinate2DMake([coder decodeDoubleForKey:@"northEastLatitude"],
                                                           [coder decodeDoubleForKey:@"northEastLongitude"]);
    MGLCoordinateBounds bounds = MGLCoordinateBoundsMake(sw, ne);
    double minimumZoomLevel = [coder decodeDoubleForKey:@"minimumZoomLevel"];
    double maximumZoomLevel = [coder decodeDoubleForKey:@"maximumZoomLevel"];
    NSArray *tileList = [coder decodeObjectForKey:@"tileList"];
    MGLTileListOfflineRegion *result = [self initWithStyleURL:styleURL bounds:bounds fromZoomLevel:minimumZoomLevel toZoomLevel:maximumZoomLevel tileList:tileList];
    result.includesIdeographicGlyphs = [coder decodeBoolForKey:@"includesIdeographicGlyphs"];
    return result;
}

- (void)encodeWithCoder:(NSCoder *)coder {
    [coder encodeObject:_styleURL forKey:@"styleURL"];
    [coder encodeDouble:_bounds.sw.latitude forKey:@"southWestLatitude"];
    [coder encodeDouble:_bounds.sw.longitude forKey:@"southWestLongitude"];
    [coder encodeDouble:_bounds.ne.latitude forKey:@"northEastLatitude"];
    [coder encodeDouble:_bounds.ne.longitude forKey:@"northEastLongitude"];
    [coder encodeDouble:_maximumZoomLevel forKey:@"maximumZoomLevel"];
    [coder encodeDouble:_minimumZoomLevel forKey:@"minimumZoomLevel"];
    [coder encodeBool:_includesIdeographicGlyphs forKey:@"includesIdeographicGlyphs"];
    [coder encodeObject:_tileList forKey:@"tileList"];
}

- (id)copyWithZone:(nullable NSZone *)zone {
    MGLTileListOfflineRegion *result = [[[self class] allocWithZone:zone] initWithStyleURL:_styleURL bounds:_bounds fromZoomLevel:_minimumZoomLevel toZoomLevel:_maximumZoomLevel tileList:_tileList];
    result.includesIdeographicGlyphs = _includesIdeographicGlyphs;
    return result;
}

- (BOOL)isEqual:(id)other {
    if (other == self) {
        return YES;
    }
    if (![other isKindOfClass:[self class]]) {
        return NO;
    }

    MGLTileListOfflineRegion *otherRegion = other;
    return (_minimumZoomLevel == otherRegion->_minimumZoomLevel
            && _maximumZoomLevel == otherRegion->_maximumZoomLevel
            && MGLCoordinateBoundsEqualToCoordinateBounds(_bounds, otherRegion->_bounds)
            && [_styleURL isEqual:otherRegion->_styleURL]
            && [_tileList isEqual:otherRegion->_tileList]
            && _includesIdeographicGlyphs == otherRegion->_includesIdeographicGlyphs);
}

- (NSUInteger)hash {
    return (_styleURL.hash
            + @(_bounds.sw.latitude).hash + @(_bounds.sw.longitude).hash
            + @(_bounds.ne.latitude).hash + @(_bounds.ne.longitude).hash
            + @(_minimumZoomLevel).hash + @(_maximumZoomLevel).hash
            + @(_includesIdeographicGlyphs).hash
            + _tileList.hash
            );
}

@end
