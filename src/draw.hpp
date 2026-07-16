// draw.h -- these are the only functions outside the refresh allowed
#pragma once
// to touch the vid buffer

#include <string_view>

namespace Draw {

extern qpic_t* draw_disc;

void Draw_Init();
void Draw_Character(int x, int y, int num);
void Draw_Pic(int x, int y, qpic_t* pic);
void Draw_TransPic(int x, int y, qpic_t* pic);
void Draw_TransPicTranslate(int x, int y, qpic_t* pic, const byte* translation);
void Draw_ConsoleBackground(int lines);
void Draw_BeginDisc();
void Draw_EndDisc();
void Draw_TileClear(int x, int y, int w, int h);
void Draw_Fill(int x, int y, int w, int h, int c);
void Draw_FadeScreen();
void Draw_String(int x, int y, std::string_view str);
inline qpic_t* Draw_PicFromWad(std::string_view name)
{
    return (qpic_t*)Wad::W_GetLumpName(std::string(name).c_str());
}
qpic_t* Draw_CachePic(std::string_view path);

} // namespace Draw

