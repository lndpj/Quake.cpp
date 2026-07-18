// model.h -- model structures (brush, alias, sprite) and loading declarations
#pragma once

#ifndef __MODEL__
#define __MODEL__

#include "modelgen.hpp"
#include "spritegn.hpp"
#include <EASTL/vector.h>

/*

d*_t structures are on-disk representations
m*_t structures are in-memory

*/

/*
==============================================================================

BRUSH MODELS

==============================================================================
*/

//
// in memory representation
//
// !!! if this is changed, it must be changed in asm_draw.h too !!!
struct mvertex_t {
    Vector3 position;
};

constexpr int SIDE_FRONT = 0;
constexpr int SIDE_BACK = 1;
constexpr int SIDE_ON = 2;

// plane_t structure
// !!! if this is changed, it must be changed in asm_i386.h too !!!
struct mplane_s {
    Vector3 normal;
    float dist;
    byte type;     // for texture axis selection and fast side tests
    byte signbits; // signx + signy<<1 + signz<<1
    byte pad[2];
};
using mplane_t = mplane_s;

struct texture_s {
    char name[16];
    unsigned width, height;
    int anim_total;                    // total tenths in sequence ( 0 = no)
    int anim_min, anim_max;            // time for this frame min <=time< max
    struct texture_s* anim_next;       // in the animation sequence
    struct texture_s* alternate_anims; // bmodels in frmae 1 use these
    unsigned offsets[MIPLEVELS];       // four mip maps stored
};
using texture_t = texture_s;

constexpr int SURF_PLANEBACK = 2;
constexpr int SURF_DRAWSKY = 4;
constexpr int SURF_DRAWSPRITE = 8;
constexpr int SURF_DRAWTURB = 0x10;
constexpr int SURF_DRAWTILED = 0x20;
constexpr int SURF_DRAWBACKGROUND = 0x40;

// !!! if this is changed, it must be changed in asm_draw.h too !!!
struct medge_t {
    unsigned short v[2];
    unsigned int cachededgeoffset;
};

struct mtexinfo_t {
    float vecs[2][4];
    float mipadjust;
    texture_t* texture;
    int flags;
};

struct msurface_s {
    int visframe; // should be drawn when node is crossed

    int dlightframe;
    int dlightbits;

    mplane_t* plane;
    int flags;

    int firstedge; // look up in model->surfedges[], negative numbers
    int numedges;  // are backwards edges

    // surface generation data
    struct surfcache_s* cachespots[MIPLEVELS];

    short texturemins[2];
    short extents[2];

    mtexinfo_t* texinfo;

    // lighting info
    byte styles[MAXLIGHTMAPS];
    byte* samples; // [numstyles*surfsize]
};
using msurface_t = msurface_s;

struct mnode_s {
    // common with leaf
    int contents; // 0, to differentiate from leafs
    int visframe; // node needs to be traversed if current

    short minmaxs[6]; // for bounding box culling

    struct mnode_s* parent;

    // node specific
    mplane_t* plane;
    struct mnode_s* children[2];

    unsigned short firstsurface;
    unsigned short numsurfaces;
};
using mnode_t = mnode_s;

struct mleaf_s {
    // common with node
    int contents; // wil be a negative contents number
    int visframe; // node needs to be traversed if current

    short minmaxs[6]; // for bounding box culling

    struct mnode_s* parent;

    // leaf specific
    byte* compressed_vis;
    efrag_t* efrags;

    msurface_t** firstmarksurface;
    int nummarksurfaces;
    int key; // BSP sequence number for leaf's contents
    byte ambient_sound_level[NUM_AMBIENTS];
};
using mleaf_t = mleaf_s;

// !!! if this is changed, it must be changed in asm_i386.h too !!!
struct hull_t {
    dclipnode_t* clipnodes;
    mplane_t* planes;
    int firstclipnode;
    int lastclipnode;
    Vector3 clip_mins;
    Vector3 clip_maxs;
};

/*
==============================================================================

SPRITE MODELS

==============================================================================
*/

// FIXME: shorten these?
struct mspriteframe_s {
    int width;
    int height;
    void* pcachespot; // remove?
    float up, down, left, right;
    byte pixels[4];
};
using mspriteframe_t = mspriteframe_s;

struct mspritegroup_t {
    int numframes;
    float* intervals;
    mspriteframe_t* frames[1];
};

struct mspriteframedesc_t {
    spriteframetype_t type;
    mspriteframe_t* frameptr;
};

struct msprite_t {
    int type;
    int maxwidth;
    int maxheight;
    int numframes;
    float beamlength; // remove?
    void* cachespot;  // remove?
    mspriteframedesc_t frames[1];
};

/*
==============================================================================

ALIAS MODELS

Alias models are position independent, so the cache manager can move them.
==============================================================================
*/

struct maliasframedesc_t {
    aliasframetype_t type;
    trivertx_t bboxmin;
    trivertx_t bboxmax;
    int frame;
    char name[16];
};

struct maliasskindesc_t {
    aliasskintype_t type;
    void* pcachespot;
    int skin;
};

struct maliasgroupframedesc_t {
    trivertx_t bboxmin;
    trivertx_t bboxmax;
    int frame;
};

struct maliasgroup_t {
    int numframes;
    int intervals;
    maliasgroupframedesc_t frames[1];
};

struct maliasskingroup_t {
    int numskins;
    int intervals;
    maliasskindesc_t skindescs[1];
};

// !!! if this is changed, it must be changed in asm_draw.h too !!!
struct mtriangle_s {
    int facesfront;
    int vertindex[3];
};
using mtriangle_t = mtriangle_s;

struct aliashdr_t {
    int model;
    int stverts;
    int skindesc;
    int triangles;
    maliasframedesc_t frames[1];
};

//===================================================================

//
// Whole model
//

enum modtype_t {
    mod_brush,
    mod_sprite,
    mod_alias
};

constexpr int EF_ROCKET = 1;    // leave a trail
constexpr int EF_GRENADE = 2;   // leave a trail
constexpr int EF_GIB = 4;       // leave a trail
constexpr int EF_ROTATE = 8;    // rotate (bonus items)
constexpr int EF_TRACER = 16;   // green split trail
constexpr int EF_ZOMGIB = 32;   // small blood trail
constexpr int EF_TRACER2 = 64;  // orange split trail + rotate
constexpr int EF_TRACER3 = 128; // purple trail

struct model_s {
    char name[MAX_QPATH];
    int needload; // bmodels and sprites don't cache normally

    modtype_t type;
    int numframes;
    synctype_t synctype;

    int flags;

    //
    // volume occupied by the model
    //
    Vector3 mins, maxs;
    float radius;

    //
    // brush model
    //
    int firstmodelsurface, nummodelsurfaces;

    int numsubmodels;
    dmodel_t* submodels;

    int numplanes;
    mplane_t* planes;

    int numleafs; // number of visible leafs, not counting 0
    mleaf_t* leafs;

    int numvertexes;
    mvertex_t* vertexes;

    int numedges;
    medge_t* edges;

    int numnodes;
    mnode_t* nodes;

    int numtexinfo;
    mtexinfo_t* texinfo;

    int numsurfaces;
    msurface_t* surfaces;

    int numsurfedges;
    int* surfedges;

    int numclipnodes;
    dclipnode_t* clipnodes;

    int nummarksurfaces;
    msurface_t** marksurfaces;

    hull_t hulls[MAX_MAP_HULLS];

    int numtextures;
    texture_t** textures;

    byte* visdata;
    byte* lightdata;
    char* entities;

    //
    // additional model data
    //
    cache_user_t cache; // only access through Mod_Extradata

    // Modern C++ / EASTL Container ownership
    eastl::vector<dmodel_t> submodels_owner;
    eastl::vector<mplane_t> planes_owner;
    eastl::vector<mleaf_t> leafs_owner;
    eastl::vector<mvertex_t> vertexes_owner;
    eastl::vector<medge_t> edges_owner;
    eastl::vector<mnode_t> nodes_owner;
    eastl::vector<mtexinfo_t> texinfo_owner;
    eastl::vector<msurface_t> surfaces_owner;
    eastl::vector<int> surfedges_owner;
    eastl::vector<dclipnode_t> clipnodes_owner;
    eastl::vector<dclipnode_t> hull0_clipnodes_owner;
    eastl::vector<msurface_t*> marksurfaces_owner;
    eastl::vector<texture_t*> textures_owner;

    eastl::vector<byte> visdata_owner;
    eastl::vector<byte> lightdata_owner;
    eastl::vector<char> entities_owner;

    // Variable sized dynamic allocations (e.g. textures, sprites)
    eastl::vector<eastl::vector<byte>> texture_allocations;
    eastl::vector<eastl::vector<byte>> sprite_allocations;
};
using model_t = model_s;

//============================================================================

namespace Model {

void Mod_Print(void);
void Mod_Init(void);
void Mod_ClearAll(void);
model_t* Mod_ForName(const char* name, qboolean crash);
void* Mod_Extradata(model_t* mod);
void Mod_TouchModel(char* name);

mleaf_t* Mod_PointInLeaf(const Vector3& p, model_t* model);
byte* Mod_LeafPVS(mleaf_t* leaf, model_t* model);

} // namespace Model

#endif // __MODEL__
