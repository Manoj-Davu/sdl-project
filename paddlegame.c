#include<stdio.h>
#include<SDL2/SDL.h>
#include<SDL2/SDL_ttf.h>


#define WIDTH 800
#define HEIGHT 600

#define BRICK_ROWS 5
#define BRICK_COLS 10

typedef struct{/* Brick structure that allow us to create a brick with a life */
SDL_Rect rect;
int live;
}Brick;

int score=0;

void Move_Paddle(SDL_Rect * rect,const Uint8 * state)
{
  int speed=8;
 
if(state[SDL_SCANCODE_LEFT])
     rect->x-=speed;/* press left Key to move left */

if(state[SDL_SCANCODE_RIGHT])
 {
     rect->x+=speed;/*Press right key to move Right */
 }
 if(rect->x<0)
     rect->x=0; /* if paddle goes less than zero ,then the paddle will stay at 0 only */
 
 if(rect->x>WIDTH-rect->w) /* if paddle goes beyond WIDTH then , the paddle will stay at WIDTH-paddle->w */
     rect->x=WIDTH-rect->w;
}
/* These are paddle boundary conditions*/

void Move_Ball(int *x,int *y,int *vx,int *vy)
{
  *x+=*vx;
  *y+=*vy;

 if(*x<=0 || *x>=WIDTH-10)
    *vx*=-1;
 if(*y<=0)
    *vy*=-1;

}


void Check_Pad_Col(int *x,int *y,int *vy,SDL_Rect *rect)
{
  SDL_Rect ball={*x,*y,10,10};

 if(SDL_HasIntersection(&ball,rect))  /* If ball and paddle makes contact then the ball's velocity will be reversed, that means the ball bounce back */
  {
    *vy*=-1;
    *y=rect->y-10;
  }
}

void Check_Bri_Col(int *vx,int *vy,int *x,int *y,Brick bricks[])
{
   SDL_Rect ball={*x,*y,10,10};
 
for(int i=0;i<BRICK_ROWS*BRICK_COLS;i++)
{

  if(bricks[i].live && SDL_HasIntersection(&ball,&bricks[i].rect))  
{   bricks[i].live=0;
       score+=2;                 /*if ball makes contact with brick ,the brick life become Zero and in next frame the brick will not render and the ball bounces back */
       *vy*=-1;
       break;
}
}}

void Draw_Bricks(SDL_Renderer *r,Brick bricks[])
{
   SDL_SetRenderDrawColor(r,0,255,0,255);
  
  for(int i=0;i<BRICK_ROWS*BRICK_COLS;i++)
     {
          if(bricks[i].live)
              {
                    SDL_RenderFillRect(r,&bricks[i].rect);
              }

     }
}
int main()
{

if(SDL_Init(SDL_INIT_VIDEO)!=0)
{
printf("INIT error :%s/n",SDL_GetError());
return 1;}

if(TTF_Init()==-1)
{
printf("TTF Error :%s\n",TTF_GetError());
return 1;}

TTF_Font * font=TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",42);

if(!font)
{
printf("Font Error:%s\n",TTF_GetError());
return 1;}



SDL_Window* window=SDL_CreateWindow("Brick Game",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,WIDTH,HEIGHT,0);
if(!window)
{
printf("Window Eroor:%s/n",SDL_GetError());
return 1;
}

SDL_Renderer * render=SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
if(!render)
{
printf("REnderer Error :%s/n",SDL_GetError());
return 1;}
SDL_Event event;
int running=1;

SDL_Rect paddle={750,HEIGHT-15,40,15};

int bx=748,by=HEIGHT-20;
int vx=3,vy=3;

Brick bricks[BRICK_ROWS*BRICK_COLS];

int bw=WIDTH/BRICK_COLS;
int bh=20;


for(int i=0;i<BRICK_ROWS;i++)
{
   for(int j=0;j<BRICK_COLS;j++)
      {
          int idx=i*BRICK_COLS+j;
          bricks[idx].rect.x=j*bw;/* Here I'm defining the brick dimensions */
          bricks[idx].rect.y=i*bh;
          bricks[idx].rect.w=bw-2;
          bricks[idx].rect.h=bh-2;
          bricks[idx].live=1;

      }

}
int paused=0;
while(running)
{
  const Uint8 *key=SDL_GetKeyboardState(NULL); 
while(SDL_PollEvent(&event))
{

if(event.type==SDL_QUIT)
running=0;
if(event.type==SDL_KEYDOWN)
  if(event.key.keysym.sym==SDLK_p)
      paused=!paused;/* if User entered  P  on keyboard .The game will pause . */
if(event.key.keysym.sym==SDLK_ESCAPE)/* If User entered ESC key on keyboard then the game will quit.  */
     running=0;
}

int gameover=0;
if(!gameover && !paused)
{
  Move_Paddle(&paddle,key);
  Move_Ball(&bx,&by,&vx,&vy);
  Check_Pad_Col(&bx,&by,&vy,&paddle);
  Check_Bri_Col(&vx,&vy,&bx,&by,bricks);
}
if(by>HEIGHT)
{

  gameover=1;


}


char scoret[100];

sprintf(scoret,"SCORE:%d",score);


if(gameover)
{
SDL_Color color={255,255,255};
SDL_Surface *surf=TTF_RenderText_Solid(font,"GAME OVER",color);

if(!surf)
{
printf("Surface Error:%s\n",TTF_GetError());
return 1;}

SDL_Texture *texture=SDL_CreateTextureFromSurface(render,surf);
SDL_Rect dst={200,200,surf->w,surf->h};


SDL_RenderCopy(render,texture,NULL,&dst);
SDL_FreeSurface(surf);
SDL_DestroyTexture(texture);
SDL_RenderPresent(render);
SDL_Delay(2000);
running=0;
}


SDL_SetRenderDrawColor(render,0,0,0,255);
SDL_RenderClear(render);

SDL_SetRenderDrawColor(render,255,0,0,255);
SDL_RenderFillRect(render,&paddle);

SDL_Rect ball={bx,by,10,10};
SDL_SetRenderDrawColor(render,255,255,255,255);
SDL_RenderFillRect(render,&ball);

Draw_Bricks(render,bricks);
SDL_Delay(16);



SDL_Color yellow={255,255,0};
SDL_Color orange={255,165,0};
SDL_Color pink={255,192,203,255};
if(score==100)
{
SDL_Surface *winsurf=TTF_RenderText_Solid(font,"YOU WON",orange);
if(!winsurf){printf("Surface Error:%s\n",TTF_GetError());return 1;}
SDL_Texture *winTexture=SDL_CreateTextureFromSurface(render,winsurf);
SDL_Rect winRect={300,300,winsurf->w,winsurf->h};
SDL_RenderCopy(render,winTexture,NULL,&winRect);
SDL_FreeSurface(winsurf);
SDL_DestroyTexture(winTexture);
SDL_RenderPresent(render);
SDL_Delay(2000);
running=0;
}

if(paused)
{
  SDL_Surface *pause=TTF_RenderText_Solid(font,"GAME PAUSED",pink);
  if(!pause){printf("PAUSE Error:%s\n",TTF_GetError());return 1;}
  SDL_Texture *pauseTexture=SDL_CreateTextureFromSurface(render,pause);
  SDL_Rect paus={200,300,pause->w,pause->h};
SDL_RenderCopy(render,pauseTexture,NULL,&paus);
SDL_FreeSurface(pause);
SDL_DestroyTexture(pauseTexture);
SDL_RenderPresent(render);
}

SDL_Surface *scSurf=TTF_RenderText_Solid(font,scoret,yellow);
if(!scSurf)
{
printf("Surface Error:%s\n",TTF_GetError());
return 1;
}
SDL_Texture *scTexture=SDL_CreateTextureFromSurface(render,scSurf);
SDL_Rect scRect={20,20,scSurf->w,scSurf->h};
SDL_RenderCopy(render,scTexture,NULL,&scRect);
SDL_FreeSurface(scSurf);
SDL_DestroyTexture(scTexture);

SDL_RenderPresent(render);


}
TTF_CloseFont(font);
TTF_Quit();
SDL_DestroyWindow(window);
SDL_DestroyRenderer(render);
SDL_Quit();

return 0;
}
