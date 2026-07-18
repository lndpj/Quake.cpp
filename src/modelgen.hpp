// modelgen.hpp -- alias model file format structures
#pragma once

#include <cstdint>
#include "mathlib.hpp"

constexpr int ALIAS_VERSION = 6;
constexpr int ALIAS_ONSEAM = 0x0020;
constexpr int DT_FACES_FRONT = 0x0010;
constexpr std::uint32_t IDPOLYHEADER = (('O' << 24) + ('P' << 16) + ('D' << 8) + 'I');

// must match definition in spritegn.hpp
#ifndef SYNCTYPE_T
#define SYNCTYPE_T
enum class synctype_t : int {
    ST_SYNC = 0,
    ST_RAND
};
#endif

enum class aliasframetype_t : int {
    ALIAS_SINGLE = 0,
    ALIAS_GROUP
};

enum class aliasskintype_t : int {
    ALIAS_SKIN_SINGLE = 0,
    ALIAS_SKIN_GROUP
};

struct mdl_t {
    int ident;
    int version;
    Vector3 scale;
    Vector3 scale_origin;
    float boundingradius;
    Vector3 eyeposition;
    int numskins;
    int skinwidth;
    int skinheight;
    int numverts;
    int numtris;
    int numframes;
    synctype_t synctype;
    int flags;
    float size;
};

struct stvert_t {
    int onseam;
    int s;
    int t;
};

struct dtriangle_t {
    int facesfront;
    int vertindex[3];
};

struct trivertx_t {
    std::uint8_t v[3];
    std::uint8_t lightnormalindex;
};

struct daliasframe_t {
    trivertx_t bboxmin;
    trivertx_t bboxmax;
    char name[16];
};

struct daliasgroup_t {
    int numframes;
    trivertx_t bboxmin;
    trivertx_t bboxmax;
};

struct daliasskingroup_t {
    int numskins;
};

struct daliasinterval_t {
    float interval;
};

struct daliasskininterval_t {
    float interval;
};

struct daliasframetype_t {
    aliasframetype_t type;
};

struct daliasskintype_t {
    aliasskintype_t type;
};
