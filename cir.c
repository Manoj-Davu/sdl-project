#include "cir.h"
#include<SDL2/SDL.h>



void Fill_Circle(SDL_Renderer *surface,int cx,int cy,int rad,SDL_Color color)

{

SDL_SetRenderDrawColor(surface,color.r,color.g,color.b,color.a);
int x=0;
int y=rad;
int p=1-rad;


while(x<=y)
{
SDL_RenderDrawLine(surface,cx-x,cy+y,cx+x,cy+y);
SDL_RenderDrawLine(surface,cx-x,cy-y,cx+x,cy-y);
SDL_RenderDrawLine(surface,cx-y,cy+x,cx+y,cy+x);
SDL_RenderDrawLine(surface,cx-y,cy-x,cx+y,cy-x);

x++;
if(p<0)
 p+=2*x+1;
else
 {
  y--;
  p+=2*(x-y)+1;
}

}
}


void draw_Rect(SDL_Renderer *surface,int x,int y,int w,int h,SDL_Color color)

{
SDL_Rect rect={x,y,w,h};
SDL_SetRenderDrawColor(surface,color.r,color.g,color.b,color.a);
SDL_RenderFillRect(surface,&rect);

}


void DrawCurveEdgeCir(SDL_Renderer *render,int s_x,int s_y,int CELL_SIZE,SDL_Color color)
{


int center_x=s_x+(CELL_SIZE/2);
int center_y=s_y+(CELL_SIZE/2);



int c1_x=s_x+ (center_x-s_x)/2;
int c1_y=s_y+(center_y-s_y)/2;
int c2_x=c1_x+(CELL_SIZE/2);
int c2_y=c1_y;
int c3_x=c1_x;
int c3_y=c1_y+(CELL_SIZE/2);
int c4_x=c2_x;
int c4_y=c3_y;
int radius=c2_x-center_x;



Fill_Circle(render,c1_x,c1_y,radius,color);
Fill_Circle(render,c2_x,c2_y,radius,color);
Fill_Circle(render,c3_x,c3_y,radius,color);
Fill_Circle(render,c4_x,c4_y,radius,color);
draw_Rect(render,c1_x,s_y,CELL_SIZE/2,CELL_SIZE,color);
draw_Rect(render,s_x,c1_y,CELL_SIZE,CELL_SIZE/2,color);


}

void Fill_Tri(SDL_Renderer *render,point v1,point v2,point v3,SDL_Color color)
{
if(v1.y>v2.y)
	swap(&v1,&v2);
if(v1.y>v3.y)
	swap(&v1,&v3);
if(v2.y>v3.y)
	swap(&v2,&v3);
SDL_SetRenderDrawColor(render,color.r,color.g,color.b,color.a);
for(int y=v1.y;y<=v3.y;y++)
	{

		float xa,xb;
		if(y<=v2.y)
			{
				xa=v1.x+(float)(v2.x-v1.x)*(y-v1.y)/(v2.y-v1.y);
				xb=v1.x+(float)(v3.x-v1.x)*(y-v1.y)/(v3.y-v1.y);
			}
		else
			{
				xa=v2.x+(float)(v3.x-v2.x)*(y-v2.y)/(v3.y-v2.y);
				xb=v1.x+(float)(v3.x-v1.x)*(y-v1.y)/(v3.y-v1.y);
			}
		if(xa>xb)
			{
				float temp=xa;
				xa=xb;
				xb=temp;
			}
		SDL_RenderDrawLine(render,(int)xa,y,(int)xb,y);
	}
}




void Draw_wheel(SDL_Renderer *render,int x,int y,int r,bool *moving,SDL_Color colorw,SDL_Color colort)
{

Fill_Circle(render,x,y,r,colorw);
Fill_Circle(render,x,y,r/7,colort);

point w1={x,y};
if(!(*moving))
	{
		point w2={x-r/4,y-(r-r/4) };
		point w3={x+r/4,y-(r-r/4) };
		Fill_Tri(render,w1,w2,w3,colort);
		point w4={x-r/4,y+(r-r/4)};
		point w5={x+r/4,y+(r-r/4)};
		Fill_Tri(render,w1,w4,w5,colort);
	}
/*w2.x=w2.y=0;
w3.x=w3.y=0;
w4.x=w4.y=0;
w5.x=w5.y=0;
*/
else
	{
		point w2={x+(r-r/4),y-r/4};
		point w3={x+(r-r/4),y+r/4};
		Fill_Tri(render,w1,w2,w3,colort);
		point w4={x-(r-r/4),y-r/4};
		point w5={x-(r-r/4),y+r/4};
		Fill_Tri(render,w1,w4,w5,colort);
/*w2.x=x+(r-r/4);
w3.x=x+(r-r/4);	w4.x=x-(r-r/4);
w5.x=x-(r-r/4);
w2.y=y-r/4;
w3.y=y+r/4;
w4.y=y-r/4;
w5.y=y+r/4;
Fill_Tri(render,w1,w2,w3,Black);
Fill_Tri(render,w1,w4,w5,Black);*/

	}

}


void Draw_Ellipse(SDL_Renderer *render,int xc,int yc,int rx,int ry)
{
int x=0;
int y=ry;
long rx2=rx*rx;
long ry2=ry*ry;
long rx22=2*rx2;
long ry22=2*ry2;


long px=0;
long py=rx22*y;
long p=ry2-(rx2*ry)+(rx2/4);
SDL_SetRenderDrawColor(render,255,255,255,255);

while(px<py)
	{
	SDL_RenderDrawLine(render,xc-x,yc+y,xc+x,yc+y);
	SDL_RenderDrawLine(render,xc-x,yc-y,xc+x,yc-y);
	x++;
	px+=ry22;
	if(p<0)
  		p+=ry2+px;
	else
		{
			y--;
			py-=rx22;
			p+=ry2+px-py;
		}
	}

p=(ry2*(x+0.5)*(x+0.5))+(rx2*(y-1)*(y-1))-(rx2*ry2);
while(y>=0)
	{
	SDL_RenderDrawLine(render,xc-x,yc+y,xc+x,yc+y);
	SDL_RenderDrawLine(render,xc-x,yc-y,xc+x,yc-y);
	y--;
	py-=rx22;
	if(p>0)
  		p+=rx2-py;
	else
		{
			x++;
			px+=ry22;
			p+=rx2-py+px;
		}
	}
}

void swap(point *a,point *b)
{
point temp=*a;
*a=*b;
*b=temp;
}


