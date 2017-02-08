//
//  MGLTileID.h
//  ios
//
//  Created by Anna Johnson on 2/7/17.
//  Copyright © 2017 Mapbox. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef struct
{
  uint32_t x, y, z;
} MGLTileID;


bool MGLTileIDsEqual(MGLTileID one, MGLTileID two);

// Returns an MGTileID tile from tile key
MGLTileID MGLTileIDFromKey(uint64_t tilekey);

/// Returns a unique key of the tile for use in the SQLite cache
uint64_t MGLTileKey(MGLTileID tile);


MGLTileID MGLTileIDMake(uint32_t x, uint32_t y, uint32_t z);
