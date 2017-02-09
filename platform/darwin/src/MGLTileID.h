#import <Foundation/Foundation.h>

#import "MGLFoundation.h"

typedef struct
{
  uint8_t z;
  uint32_t x, y;
} MGLTileID;

// Returns an MGTileID tile from tile key
FOUNDATION_EXTERN MGL_EXPORT MGLTileID MGLTileIDFromKey(uint64_t tilekey);

FOUNDATION_EXTERN MGL_EXPORT bool MGLTileIDsEqual(MGLTileID one, MGLTileID two);

// Returns a unique key of the tile
FOUNDATION_EXTERN MGL_EXPORT uint64_t MGLTileKey(MGLTileID tile);

FOUNDATION_EXTERN MGL_EXPORT MGLTileID MGLTileIDMake(uint32_t x, uint32_t y, uint8_t z);
