// sbar.h -- status bar drawing declarations
#pragma once

inline constexpr int SBAR_HEIGHT = 24;

extern int sb_lines; // scan lines to draw

namespace Sbar {

void Sbar_Init();

void Sbar_Changed();

void Sbar_Draw();

void Sbar_IntermissionOverlay();

void Sbar_FinaleOverlay();

} // namespace Sbar
