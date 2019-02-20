#import <Foundation/Foundation.h>

#import "MGLFoundation.h"

typedef struct
{
  uint8_t z;
  uint32_t x, y;
} MGLTileID;

// Returns an MGLTileId from tile key
FOUNDATION_EXTERN MGL_EXPORT MGLTileID MGLTileIDFromKey(uint64_t tileKey);

FOUNDATION_EXTERN MGL_EXPORT bool MGLTileIDsEqual(MGLTileID one, MGLTileID two);

// Returns a unique key of the tile for use in the SQLite cache
FOUNDATION_EXTERN MGL_EXPORT uint64_t MGLTileKey(MGLTileID tile);

FOUNDATION_EXTERN MGL_EXPORT MGLTileID MGLTileIDMake(uint8_t z, uint32_t x, uint32_t y);
