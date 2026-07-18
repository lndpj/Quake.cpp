// r_local.hpp -- private refresh defs
#pragma once
#include <EASTL/array.h>
#include <cstdint>
#include "r_shared.hpp"

constexpr double ALIAS_BASE_SIZE_RATIO = 1.0 / 11.0;
// normalizing factor so player model works out to about
//  1 pixel per triangle

constexpr int BMODEL_FULLY_CLIPPED = 0x10; // value returned by R_BmodelCheckBBox ()

//  if bbox is trivially rejected

//===========================================================================
// viewmodel lighting

struct alight_t {
    int ambientlight = 0;
    int shadelight = 0;
    float* plightvec = nullptr;
};

//===========================================================================
// clipped bmodel edges

struct bedge_t {
    eastl::array<mvertex_t*, 2> v{};
    bedge_t* pnext = nullptr;
};

struct auxvert_t {
    eastl::array<float, 3> fv{}; // viewspace x, y
};

//===========================================================================

namespace Render {

extern cvar_t r_clearcolor;
extern cvar_t r_drawflat;

constexpr double XCENTERING = 1.0 / 2.0;
constexpr double YCENTERING = 1.0 / 2.0;

constexpr double CLIP_EPSILON = 0.001;

constexpr double BACKFACE_EPSILON = 0.01;

//===========================================================================

constexpr int DIST_NOT_SET = 98765;

// !!! if this is changed, it must be changed in asm_draw.h too !!!
struct clipplane_t {
    Vector3 normal{};
    float dist = 0.0f;
    clipplane_t* next = nullptr;
    uint8_t leftedge = 0;
    uint8_t rightedge = 0;
    eastl::array<uint8_t, 2> reserved{};
};

//=============================================================================

void R_RenderWorld(void);

//=============================================================================

extern Vector3 r_origin;

//=============================================================================

void R_ClearPolyList(void);
void R_DrawPolyList(void);

//
// current entity info
//
extern qboolean insubmodel;

void R_DrawSprite(void);
void R_RenderFace(msurface_t* fa, int clipflags);
void R_RenderPoly(msurface_t* fa, int clipflags);
void R_RenderBmodelFace(bedge_t* pedges, msurface_t* psurf);
void R_TransformFrustum(void);
void R_SetSkyFrame(void);
void R_DrawSurfaceBlock16(void);
void R_DrawSurfaceBlock8(void);
texture_t* R_TextureAnimation(texture_t* base);

void R_DrawSubmodelPolygons(model_t* pmodel, int clipflags);
void R_DrawSolidClippedSubmodelPolygons(model_t* pmodel);

void R_AddPolygonEdges(emitpoint_t* pverts, int numverts, int miplevel);
surf_t* R_GetSurf(void);
void R_AliasDrawModel(alight_t* plighting);
void R_BeginEdgeFrame(void);
void R_ScanEdges(void);
void D_DrawSurfaces(void);
void R_InsertNewEdges(edge_t* edgestoadd, edge_t* edgelist);
void R_StepActiveU(edge_t* pedge);
void R_RemoveEdges(edge_t* pedge);

extern void R_RotateBmodel(void);

// !!! if this is changed, it must be changed in asm_draw.h too !!!
constexpr double NEAR_CLIP = 0.01;

constexpr int MAXBVERTINDEXES = 1000; // new clipped vertices when clipping bmodels
//  to the world BSP

struct btofpoly_t {
    int clipflags = 0;
    msurface_t* psurf = nullptr;
};

constexpr int MAX_BTOFPOLYS = 5000; // FIXME: tune this



void R_InitTurb(void);
void R_ZDrawSubmodelPolys(model_t* clmodel);

//=========================================================
// Alias models
//=========================================================

constexpr int MAXALIASVERTS = 2000; // TODO: tune this
constexpr int ALIAS_Z_CLIP_PLANE = 5;



qboolean R_AliasCheckBBox(void);

//=========================================================
// turbulence stuff

constexpr int AMP = 8 * 0x10000;
constexpr int AMP2 = 3;
constexpr int SPEED = 20;

//=========================================================
// particle stuff

void R_DrawParticles(void);
void R_InitParticles(void);
void R_ClearParticles(void);
void R_ReadPointFile_f(void);



extern int screenwidth;



void R_AliasClipTriangle(mtriangle_t* ptri);



void R_StoreEfrags(efrag_t** ppefrag);
void R_TimeRefresh_f(void);
void R_TimeGraph(void);
void R_PrintAliasStats(void);
void R_PrintTimes(void);
void R_PrintDSpeeds(void);
void R_AnimateLight(void);
int R_LightPoint(const Vector3& p);
void R_SetupFrame(void);
void R_cshift_f(void);
void R_EmitEdge(mvertex_t* pv0, mvertex_t* pv1);
void R_ClipEdge(mvertex_t* pv0, mvertex_t* pv1, clipplane_t* clip);
void R_SplitEntityOnNode2(mnode_t* node);
void R_MarkLights(dlight_t* light, int bit, mnode_t* node);

} // namespace Render
