// chase.cpp -- chase camera code

#include "quakedef.hpp"

using namespace CDAudio;
using namespace Client;
using namespace Common;
using namespace Console;
using namespace Render;
using namespace Draw;
using namespace Host;
using namespace Input;
using namespace Keys;
using namespace Math;
using namespace Menu;
using namespace Model;
using namespace Net;
using namespace VM;
using namespace Sbar;
using namespace Screen;
using namespace Server;
using namespace Audio;
using namespace Vid;
using namespace View;
using namespace Wad;
using namespace Cvar;
using namespace Cmd;


namespace Client {

cvar_t chase_back = { "chase_back", "100" };
cvar_t chase_up = { "chase_up", "16" };
cvar_t chase_right = { "chase_right", "0" };
cvar_t chase_active = { "chase_active", "0" };

vec3_t chase_pos;
vec3_t chase_angles;

vec3_t chase_dest;
vec3_t chase_dest_angles;

void Chase_Init(void)
{
    Cvar::Register(&chase_back);
    Cvar::Register(&chase_up);
    Cvar::Register(&chase_right);
    Cvar::Register(&chase_active);
}

void TraceLine(vec3_t start, vec3_t end, vec3_t impact)
{
    trace_t trace;

    memset(&trace, 0, sizeof(trace));
    SV_RecursiveHullCheck(cl.worldmodel->hulls, 0, 0, 1, start, end, &trace);

    VectorCopy(trace.endpos, impact);
}

void Chase_Update(void)
{
    int i;
    float dist;
    vec3_t forward, up, right;
    vec3_t dest, stop;

    // if can't see player, reset
    AngleVectors(cl.viewangles, forward, right, up);

    // calc exact destination
    for (i = 0; i < 3; i++) {
        chase_dest[i] = r_refdef.vieworg[i] - forward[i] * chase_back.value - right[i] * chase_right.value;
    }
    chase_dest[2] = r_refdef.vieworg[2] + chase_up.value;

    // find the spot the player is looking at
    VectorMA(r_refdef.vieworg, 4096, forward, dest);
    TraceLine(r_refdef.vieworg, dest, stop);

    // calculate pitch to look at the same spot from camera
    VectorSubtract(stop, r_refdef.vieworg, stop);
    dist = DotProduct(stop, forward);
    if (dist < 1) {
        dist = 1;
    }

    r_refdef.viewangles[PITCH] = -atan(stop[2] / dist) / M_PI * 180;

    // move towards destination
    VectorCopy(chase_dest, r_refdef.vieworg);
}

} // namespace Client
