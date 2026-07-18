// spritegn.hpp -- sprite file format structures
#pragma once

#include <cstdint>

constexpr int SPRITE_VERSION = 1;

// must match definition in modelgen.hpp
#ifndef SYNCTYPE_T
#define SYNCTYPE_T
enum class synctype_t : int {
    ST_SYNC = 0,
    ST_RAND
};
#endif

constexpr int SPR_VP_PARALLEL_UPRIGHT = 0;
constexpr int SPR_FACING_UPRIGHT = 1;
constexpr int SPR_VP_PARALLEL = 2;
constexpr int SPR_ORIENTED = 3;
constexpr int SPR_VP_PARALLEL_ORIENTED = 4;

enum class spriteframetype_t : int {
    SPR_SINGLE = 0,
    SPR_GROUP
};

struct dsprite_t {
    int ident;
    int version;
    int type;
    float boundingradius;
    int width;
    int height;
    int numframes;
    float beamlength;
    synctype_t synctype;
};

struct dspriteframe_t {
    int origin[2];
    int width;
    int height;
};

struct dspritegroup_t {
    int numframes;
};

struct dspriteinterval_t {
    float interval;
};

struct dspriteframetype_t {
    spriteframetype_t type;
};

constexpr std::uint32_t IDSPRITEHEADER = (('P' << 24) + ('S' << 16) + ('D' << 8) + 'I');
// little-endian "IDSP"
