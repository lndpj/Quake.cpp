// renderer.hpp -- public interface to refresh functions
#pragma once

constexpr int MAXCLIPPLANES = 11;

constexpr int TOP_RANGE = 16; // soldier uniform colors
constexpr int BOTTOM_RANGE = 96;

//=============================================================================

struct efrag_s {
    struct mleaf_s* leaf;
    efrag_s* leafnext;
    struct entity_t* entity;
    efrag_s* entnext;
};
using efrag_t = efrag_s;

struct entity_t {
    bool forcelink; // model changed

    int update_type;

    entity_state_t baseline; // to fill in defaults in updates

    double msgtime;        // time of last update
    Vector3 msg_origins[2]; // last two updates (0 is newest)
    Vector3 origin;
    Vector3 msg_angles[2]; // last two updates (0 is newest)
    Vector3 angles;
    struct model_s* model; // nullptr = no model
    efrag_s* efrag; // linked list of efrags
    int frame;
    float syncbase; // for client-side animations
    byte* colormap;
    int effects;  // light, particals, etc
    int skinnum;  // for Alias models
    int visframe; // last frame this entity was found in an active leaf

    int dlightframe; // dynamic lighting
    int dlightbits;

    // FIXME: could turn these into a union
    int trivial_accept;
    struct mnode_s* topnode; // for bmodels, first world node that splits bmodel, or nullptr if not split
};
using entity_s = entity_t;

// !!! if this is changed, it must be changed in asm_draw.h too !!!
struct refdef_t {
    vrect_t vrect; // subwindow in video for refresh
    // FIXME: not need vrect next field here?
    vrect_t aliasvrect;                    // scaled Alias version
    int vrectright, vrectbottom;           // right & bottom screen coords
    int aliasvrectright, aliasvrectbottom; // scaled Alias versions
    float vrectrightedge;                  // rightmost right edge we care about,
    //  for use in edge list
    float fvrectx, fvrecty;         // for floating-point compares
    float fvrectx_adj, fvrecty_adj; // left and top edges, for clamping
    int64_t vrect_x_adj_shift20;        // (vrect.x + 0.5 - epsilon) << 20
    int64_t vrectright_adj_shift20;     // (vrectright + 0.5 - epsilon) << 20
    float fvrectright_adj, fvrectbottom_adj;
    // right and bottom edges, for clamping
    float fvrectright;           // rightmost edge, for Alias clamping
    float fvrectbottom;          // bottommost edge, for Alias clamping
    float horizontalFieldOfView; // at Z = 1.0, this many X is visible
    // 2.0 = 90 degrees
    float xOrigin; // should probably allways be 0.5
    float yOrigin; // between be around 0.3 to 0.5

    Vector3 vieworg;
    Vector3 viewangles;

    float fov_x, fov_y;

    int ambientlight;
};

struct texture_s;

namespace Render {

//
// refresh
//

extern refdef_t r_refdef;
extern Vector3 r_origin, vpn, vright, vup;

extern texture_s* r_notexture_mip;

void R_Init();
void R_InitTextures();
void R_InitEfrags();
void R_RenderView();
void R_ViewChanged(vrect_t* pvrect, int lineadj, float aspect);
void R_InitSky(texture_s* mt);

void R_AddEfrags(entity_t* ent);
void R_RemoveEfrags(entity_t* ent);

void R_NewMap();

void R_ParseParticleEffect();
void R_RunParticleEffect(const Vector3& org, const Vector3& dir, int color, int count);
void R_RocketTrail(Vector3 start, const Vector3& end, int type);

void R_EntityParticles(entity_t* ent);
void R_BlobExplosion(const Vector3& org);
void R_ParticleExplosion(const Vector3& org);
void R_ParticleExplosion2(const Vector3& org, int colorStart, int colorLength);
void R_LavaSplash(const Vector3& org);
void R_TeleportSplash(const Vector3& org);

void R_PushDlights();

//
// surface cache related
//
extern bool r_cache_thrash;

int D_SurfaceCacheForRes(int width, int height);
void D_FlushCaches();
void D_DeleteSurfaceCache();
void D_InitCaches(void* buffer, int size);
void R_SetVrect(vrect_t* pvrect, vrect_t* pvrectin, int lineadj);

} // namespace Render
