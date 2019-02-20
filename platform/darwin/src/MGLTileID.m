//
//  MGLTileID.m
//  ios
//
//  Created by Anna Johnson on 2/7/17.
//  Copyright © 2017 Mapbox. All rights reserved.
//

#import "MGLTileID.h"

bool MGLTileIDsEqual(MGLTileID one, MGLTileID two) {
  return (one.x == two.x) && (one.y == two.y) && (one.z == two.z);
}

MGLTileID MGLTileIDFromKey(uint64_t tileKey) {
    MGLTileID t;
    t.z = tileKey >> 56;
    t.x = tileKey >> 28 & 0xFFFFFFFLL;
    t.y = tileKey & 0xFFFFFFFLL;
    return t;
}


uint64_t MGLTileKey(MGLTileID tile) {
  uint64_t zoom = (uint64_t) tile.z & 0xFFLL; // 8bits, 256 levels
  uint64_t x = (uint64_t) tile.x & 0xFFFFFFFLL;  // 28 bits
  uint64_t y = (uint64_t) tile.y & 0xFFFFFFFLL;  // 28 bits
  
  uint64_t key = (zoom << 56) | (x << 28) | (y << 0);
  
  return key;
}


MGLTileID MGLTileIDMake(uint8_t z, uint32_t x, uint32_t y) {
  MGLTileID t;
  t.x = x;
  t.y = y;
  t.z = z;
  return t;
}
