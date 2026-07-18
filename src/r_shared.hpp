// r_shared.hpp: general refresh-related stuff shared between the refresh and the driver
#pragma once
#include <EASTL/array.h>
#include <cstdint>

constexpr int MAXVERTS = 16;                    // max points in a surface polygon
constexpr int MAXWORKINGVERTS = MAXVERTS + 4; // max points in an intermediate
//  polygon (while processing)
// !!! if this is changed, it must be changed in d_ifacea.h too !!!
constexpr int MAXHEIGHT = 2160;
constexpr int MAXWIDTH = 3840;
constexpr int MAXDIMENSION = (MAXHEIGHT > MAXWIDTH) ? MAXHEIGHT : MAXWIDTH;

constexpr int CYCLE = 256;
constexpr int SIN_BUFFER_SIZE = MAXDIMENSION + CYCLE;

constexpr int INFINITE_DISTANCE = 0x10000; // distance that's always guaranteed to
//  be farther away than anything in
//  the scene

//===================================================================

constexpr int NUMSTACKEDGES = 2400;
constexpr int MINEDGES = NUMSTACKEDGES;
constexpr int NUMSTACKSURFACES = 800;
constexpr int MINSURFACES = NUMSTACKSURFACES;
constexpr int MAXSPANS = 8000;

// !!! if this is changed, it must be changed in asm_draw.h too !!!
struct espan_t {
    int u = 0, v = 0, count = 0;
    espan_t* pnext = nullptr;
};

struct surf_t {
    surf_t* next = nullptr;
    surf_t* prev = nullptr;
    espan_t* spans = nullptr;
    int key = 0;
    int last_u = 0;
    int spanstate = 0;
    int flags = 0;
    void* data = nullptr;
    entity_t* entity = nullptr;
    float nearzi = 0.0f;
    qboolean insubmodel = {};
    float d_ziorigin = 0.0f, d_zistepu = 0.0f, d_zistepv = 0.0f;
    eastl::array<int, 2> pad{};
};

// flags in finalvert_t.flags
constexpr int ALIAS_LEFT_CLIP = 0x0001;
constexpr int ALIAS_TOP_CLIP = 0x0002;
constexpr int ALIAS_RIGHT_CLIP = 0x0004;
constexpr int ALIAS_BOTTOM_CLIP = 0x0008;
constexpr int ALIAS_Z_CLIP = 0x0010;
// ALIAS_ONSEAM is defined in modelgen.hpp (0x0020)
constexpr int ALIAS_XY_CLIP_MASK = 0x000F;

// !!! if this is changed, it must be changed in asm_draw.h too !!!
struct edge_t {
    int64_t u = 0;
    int64_t u_step = 0;
    edge_t* prev = nullptr;
    edge_t* next = nullptr;
    eastl::array<unsigned short, 2> surfs{};
    edge_t* nextremove = nullptr;
    float nearzi = 0.0f;
    medge_t* owner = nullptr;
};

namespace Render {

extern void R_DrawLine(polyvert_t* polyvert0, polyvert_t* polyvert1);

extern int screenwidth;

extern float pixelAspect;

extern int r_drawnpolycount;

extern eastl::array<int, SIN_BUFFER_SIZE> sintable;
extern eastl::array<int, SIN_BUFFER_SIZE> intsintable;

extern Vector3 vup, base_vup;
extern Vector3 vpn, base_vpn;
extern Vector3 vright, base_vright;
extern entity_t* currententity;

extern surf_t *surfaces, *surface_p, *surf_max;

extern Vector3 sxformaxis[4];
extern Vector3 txformaxis[4];

extern Vector3 modelorg, base_modelorg;

extern float xcenter, ycenter;
extern float xscale, yscale;
extern float xscaleinv, yscaleinv;
extern float xscaleshrink, yscaleshrink;

extern eastl::array<int, 256> d_lightstylevalue;

extern void TransformVector(const Vector3& in, Vector3& out);
extern void SetUpForLineScan(fixed8_t startvertu,
    fixed8_t startvertv,
    fixed8_t endvertu,
    fixed8_t endvertv);

extern int r_skymade;
extern void R_MakeSky(void);

} // namespace Render
