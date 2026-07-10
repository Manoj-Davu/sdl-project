#ifndef CIR_H
#define CIR_H

#include <SDL2/SDL.h>

typedef struct
{ int x;
int y;
}point;

void Fill_Circle(SDL_Renderer *surface,int cx,int cy,int rad,SDL_Color color);

void draw_Rect(SDL_Renderer *surface,int x,int y,int w,int h,SDL_Color color);


void DrawCurveEdgeCir(SDL_Renderer *surface,int s_x,int s_y,int CELL_SIZE,SDL_Color color);

void Fill_Tri(SDL_Renderer *render,point v1,point v2,point v3,SDL_Color color);

void Draw_wheel(SDL_Renderer *render,int x,int y,int r,bool *moving,SDL_Color colorw,SDL_Color colort);

void Draw_Ellipse(SDL_Renderer *render,int xc,int yc,int rx,int ry);

void swap(point *a,point *b);

#endif
