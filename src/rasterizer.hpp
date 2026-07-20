// rasterizer.hpp: interface header file for rasterization driver modules
#pragma once

#include <EASTL/array.h>

constexpr int WARP_WIDTH = 320;
constexpr int WARP_HEIGHT = 200;

constexpr int MAX_LBM_HEIGHT = 480;

struct emitpoint_t {
    float u = 0.0f;
    float v = 0.0f;
    float s = 0.0f;
    float t = 0.0f;
    float zi = 0.0f;
};

enum class ptype_t {
    Static,
    Grav,
    SlowGrav,
    Fire,
    Explode,
    Explode2,
    Blob,
    Blob2
};

// !!! if this is changed, it must be changed in d_ifacea.h too !!!
struct particle_t {
    // driver-usable fields
    Vector3 org{};
    float color = 0.0f;
    // drivers never touch the following fields
    particle_t* next = nullptr;
    Vector3 vel{};
    float ramp = 0.0f;
    float die = 0.0f;
    ptype_t type = ptype_t::Static;
};

constexpr float PARTICLE_Z_CLIP = 8.0f;

struct polyvert_t {
    float u = 0.0f;
    float v = 0.0f;
    float zi = 0.0f;
    float s = 0.0f;
    float t = 0.0f;
};

struct polydesc_t {
    int numverts = 0;
    float nearzi = 0.0f;
    msurface_t* pcurrentface = nullptr;
    polyvert_t* pverts = nullptr;
};

// !!! if this is changed, it must be changed in d_ifacea.h too !!!
struct finalvert_t {
    eastl::array<int, 6> v{}; // u, v, s, t, l, 1/z
    int flags = 0;
    float reserved = 0.0f;
};

// !!! if this is changed, it must be changed in d_ifacea.h too !!!
struct affinetridesc_t {
    void* pskin = nullptr;
    maliasskindesc_t* pskindesc = nullptr;
    int skinwidth = 0;
    int skinheight = 0;
    mtriangle_t* ptriangles = nullptr;
    finalvert_t* pfinalverts = nullptr;
    int numtriangles = 0;
    int drawtype = 0;
    int seamfixupX16 = 0;
};

// !!! if this is changed, it must be changed in d_ifacea.h too !!!
struct screenpart_t {
    float u = 0.0f;
    float v = 0.0f;
    float zi = 0.0f;
    float color = 0.0f;
};

struct spritedesc_t {
    int nump = 0;
    emitpoint_t* pverts = nullptr; // there's room for an extra element at [nump],
    //  if the driver wants to duplicate element [0] at
    //  element [nump] to avoid dealing with wrapping
    mspriteframe_t* pspriteframe = nullptr;
    Vector3 vup{}, vright{}, vpn{}; // in worldspace
    float nearzi = 0.0f;
};

struct zpointdesc_t {
    int u = 0;
    int v = 0;
    float zi = 0.0f;
    int color = 0;
};

constexpr int SKYSHIFT = 7;
constexpr int SKYSIZE = 1 << SKYSHIFT;
constexpr int SKYMASK = SKYSIZE - 1;

namespace Render {

extern cvar_t r_drawflat;
extern int d_spanpixcount;
extern int r_framecount;
extern qboolean r_drawpolys;
extern qboolean r_drawculledpolys;
extern qboolean r_worldpolysbacktofront;
extern qboolean r_recursiveaffinetriangles;
extern float r_aliasuvscale;
extern int r_pixbytes;
extern qboolean r_dowarp;

extern affinetridesc_t r_affinetridesc;
extern spritedesc_t r_spritedesc;
extern zpointdesc_t r_zpointdesc;
extern polydesc_t r_polydesc;

extern int d_con_indirect;

extern Vector3 r_pright, r_pup, r_ppn;

void D_Aff8Patch(void* pcolormap);
inline void D_EnableBackBufferAccess()
{
    VID_LockBuffer();
}

inline void D_DisableBackBufferAccess()
{
    VID_UnlockBuffer();
}

void D_PolysetDraw();
void D_PolysetDrawFinalVerts(finalvert_t* fv, int numverts);
void D_DrawParticle(particle_t* pparticle);
void D_DrawPoly();
void D_DrawSprite();
void D_DrawSurfaces();
void D_EndParticles();
void D_Init();
void D_ViewChanged();
void D_SetupFrame();
void D_StartParticles();
void D_TurnZOn();
void D_WarpScreen();

void D_DrawRect();
void D_UpdateRects(vrect_t* prect);

void D_PolysetUpdateTables();

extern int r_skydirect;
extern byte* r_skysource;

// transparency types for D_DrawRect ()
constexpr int DR_SOLID = 0;
constexpr int DR_TRANSPARENT = 1;
constexpr int TRANSPARENT_COLOR = 0xFF;

extern void* acolormap; // FIXME: should go away

//=======================================================================//

// callbacks to Quake

struct drawsurf_t {
    pixel_t* surfdat = nullptr; // destination for generated surface
    int rowbytes = 0;     // destination logical width in bytes
    msurface_t* surf = nullptr; // description for surface to generate
    eastl::array<fixed8_t, MAXLIGHTMAPS> lightadj{};
    // adjust for lightmap levels for dynamic lighting
    texture_t* texture = nullptr; // corrected for animating textures
    int surfmip = 0;        // mipmapped ratio of surface texels / world pixels
    int surfwidth = 0;      // in mipmapped texels
    int surfheight = 0;     // in mipmapped texels
};

extern drawsurf_t r_drawsurf;

void R_DrawSurface();

extern float skyspeed, skyspeed2;
extern float skytime;

extern int c_surf;
extern byte* r_warpbuffer;

} // namespace Render
