// d_local.hpp:  private rasterization driver defs
#pragma once

#include "r_shared.hpp"

//
// TODO: fine-tune this; it's based on providing some overage even if there
// is a 2k-wide scan, with subdivision every 8, for 256 spans of 12 bytes each
//
constexpr int SCANBUFFERPAD = 0x1000;

constexpr int R_SKY_SMASK = 0x007F0000;
constexpr int R_SKY_TMASK = 0x007F0000;

constexpr int DS_SPAN_LIST_END = -128;

constexpr int SURFCACHE_SIZE_AT_320X200 = 600 * 1024;

struct surfcache_s {
    surfcache_s* next;
    surfcache_s** owner; // NULL is an empty chunk of memory
    int lightadj[MAXLIGHTMAPS]; // checked for strobe flush
    int dlight;
    int size; // including header
    unsigned width;
    unsigned height; // DEBUG only needed for debug
    float mipscale;
    texture_t* texture; // checked for animating textures
    byte data[4];              // width*height elements
};
using surfcache_t = surfcache_s;

// !!! if this is changed, it must be changed in asm_draw.h too !!!
struct sspan_s {
    int u, v, count;
};
using sspan_t = sspan_s;

namespace Render {

void D_DrawSpans8(espan_t* pspans);
void D_DrawSpans16(espan_t* pspans);
void D_DrawZSpans(espan_t* pspans);
void Turbulent8(espan_t* pspan);
void D_SpriteDrawSpans(sspan_t* pspan);

void D_DrawSkyScans8(espan_t* pspan);
void D_DrawSkyScans16(espan_t* pspan);

void R_ShowSubDiv();
surfcache_t* D_CacheSurface(msurface_t* surface, int mip_level);

extern short* d_pzbuffer;

} // namespace Render
