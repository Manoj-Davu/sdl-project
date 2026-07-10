#include<SDL2/SDL.h>//For rendering or drawing
#include<SDL2/SDL_mixer.h>// For mixing Audio
#include<SDL2/SDL_ttf.h>//For text rendering
#include<stdio.h>//For Input and output
#include<stdlib.h>//DMA
#include<time.h>// For time based logics
#include<stdbool.h>// To use boolean datatype
#include<stdint.h>//To use datatypes like Uint8,Uint32,int64_t,...., etc.,
#include "cir.h"


//Dimensions of Window,cells
#define WIDTH 900
#define HEIGHT 600
#define CELL_SIZE 30
#define LN_WIDTH 2
#define ROWS (WIDTH/CELL_SIZE)
#define COLUMNS (HEIGHT/CELL_SIZE)


/* Declaring Global variables for
 Audio mixing and audio playing */
Mix_Chunk *gEatSound=NULL;
Mix_Music *bgm=NULL;
Mix_Chunk *gameover=NULL;
Mix_Chunk *hit_wall=NULL;
Mix_Chunk *hit_body=NULL;

SDL_Color white ={255,255,255,255};
SDL_Color pink ={255,20,147,255};
SDL_Color black={0,0,0,255};

bool eat=false;
//Snake Structure
typedef struct Snake_Body
{

SDL_Rect s_ele; /* It is in SDL_Rect because we need snake.h,snake.w to draw snake
and also easy to draw by using SDL_FillRect(); */
struct Snake_Body *snext;

}Snake_Body;

//Structure for food
typedef struct Food
{
SDL_Rect F_ele;
struct Food *fnext;
}Food;


/*void draw_grid(SDL_Renderer *surface)
{

 SDL_Rect line={0,0,900,LN_WIDTH};
 SDL_SetRenderDrawColor(surface,0,0,0,255);

    for(line.y=0;line.y<HEIGHT;line.y+=CELL_SIZE)
              SDL_RenderFillRect(surface,&line);

 SDL_Rect cline={0,0,LN_WIDTH,600};

    for(cline.x=0;cline.x<WIDTH;cline.x+=CELL_SIZE)
              SDL_RenderFillRect(surface,&cline);

}

*/
void draw_Food(SDL_Renderer *surface,Food *food,Snake_Body* snake_head)
{

 Fill_Circle(surface,(food->F_ele.x+(CELL_SIZE/2)),(food->F_ele.y+(CELL_SIZE/2)),CELL_SIZE/3,pink);

}

Snake_Body *if_eat_food(SDL_Renderer *surface,Snake_Body *head,Food *food)
{


 if(head->s_ele.x==food->F_ele.x && head->s_ele.y==food->F_ele.y)
    {
      eat=true;
      bool food_on_snake=true;
      while(food_on_snake)
      	{
	   int pw=(WIDTH/CELL_SIZE)-2;
           int ph=(HEIGHT/CELL_SIZE)-2;
	   int ran_x=(1+(rand()%pw))*CELL_SIZE;
	   int ran_y=(1+(rand()%ph))*CELL_SIZE;
	   food_on_snake=false;
	   food->F_ele.x=ran_x;
	   food->F_ele.y=ran_y;

	   Snake_Body *temp=head;
	   while(temp!=NULL)
		{

                   if(temp->s_ele.x==food->F_ele.x && temp->s_ele.y==food->F_ele.y)
                         {
                             food_on_snake=true;
                             break;
                         }
                   temp=temp->snext;
                 }
	}



	Snake_Body *new_body=(Snake_Body*)malloc(sizeof(Snake_Body));
	if(new_body==NULL)
	   	{
	           return head;
                }

	new_body->s_ele=(SDL_Rect){-CELL_SIZE,-CELL_SIZE,CELL_SIZE,CELL_SIZE};
	Snake_Body* tail=head;
	while(tail->snext!=NULL)
   		tail=tail->snext;


	tail->snext=new_body;

	new_body->snext=NULL;


	if(gEatSound!=NULL)
		{
		   Mix_PlayChannel(-1,gEatSound,0);

		}
    }
return head;
}



int if_snake_collide(Snake_Body *head,int alive)
{
Snake_Body *temp=head->snext->snext;
while(temp!=NULL)
	{
	   if(head->s_ele.x==temp->s_ele.x && head->s_ele.y==temp->s_ele.y)
     		{
		   if(hit_body!=NULL)
  			{
			   Mix_PlayChannel(-1,hit_body,0);
			   SDL_Delay(2000);
			}
		   alive=0;
		}

	   temp=temp->snext;
	}

if(head->s_ele.x<CELL_SIZE || head->s_ele.x>=WIDTH-CELL_SIZE || head->s_ele.y<CELL_SIZE || head->s_ele.y>=HEIGHT-CELL_SIZE)
{
	if(hit_wall!=NULL)
  		{
		   Mix_PlayChannel(-1,hit_wall,0);
		   SDL_Delay(1000);
		}
	alive=0;
}

return alive;
}

void clean_up_audio()
{

if(gEatSound!=NULL)
{
	Mix_FreeChunk(gEatSound);
	gEatSound=NULL;
}
if(bgm!=NULL)
{
	Mix_FreeMusic(bgm);
	bgm=NULL;
}

if(gameover!=NULL)
{
	Mix_FreeChunk(gameover);
	gameover=NULL;
}
if(hit_wall!=NULL)
{
	Mix_FreeChunk(hit_wall);
	hit_wall=NULL;
}

if(hit_body!=NULL)
{
	Mix_FreeChunk(hit_body);
	hit_body=NULL;
}
Mix_CloseAudio();
Mix_Quit();

}

void show_boundary(SDL_Renderer* surface,SDL_Rect  up,SDL_Rect down,SDL_Rect Lb,SDL_Rect Rb)
{
SDL_SetRenderDrawColor(surface,255,255,0,255);
SDL_RenderFillRect(surface,&up);
SDL_RenderFillRect(surface,&down);
SDL_RenderFillRect(surface,&Lb);
SDL_RenderFillRect(surface,&Rb);

}




int main()
{
srand(time(NULL));

if( SDL_Init(SDL_INIT_VIDEO)!=0)
   {
    printf("%s error",SDL_GetError());
    return 1;
   }
SDL_Window* window=SDL_CreateWindow("SNAKE GAME",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,WIDTH,HEIGHT,0);
if(window==NULL)
  {
    printf("%s Error occured\n",SDL_GetError());
    return 1;
  }



SDL_Renderer *surface=SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
if(surface==NULL)
  {
    printf("%s Error occured\n",SDL_GetError());
    return 1;
  }

if(TTF_Init()==-1)
{
	printf("Font Error:%s\n",TTF_GetError());
	return 1;
}

TTF_Font *font=TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",24);
if(font==NULL)
{
	printf("Font Error:%s\n",TTF_GetError());
	return 1;
}

int audio_flags=MIX_INIT_MP3;
if((Mix_Init(audio_flags)& audio_flags)!=audio_flags)
{
	printf("Mix_Init : failed to init MP3 support ! Error:%s\n",Mix_GetError());

}

if(Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,2048)<0)
{
	printf("SDL_mixer initialization failed ! Error :%s\n",Mix_GetError());
	SDL_Quit();
	return 1;
}

gEatSound =Mix_LoadWAV("eat.mp3");
if(gEatSound==NULL)
{
	printf("Failed to load eat.mp3! Error :%s\n",Mix_GetError());

}

bgm=Mix_LoadMUS("bgm.mp3");
if(bgm==NULL)
{
	printf("Failed to load bgm.mp3! Error :%s\n",Mix_GetError());
}

gameover=Mix_LoadWAV("game_over.mp3");
if(gameover==NULL)
{
	printf("Failed to load game_over.mp3 ! Error:%s\n",Mix_GetError());
}
hit_wall=Mix_LoadWAV("hit_wall.mp3");
if(hit_wall==NULL)
{
	printf("Failed to load hit_wall.mp3! Error:%s",Mix_GetError());
}

hit_body=Mix_LoadWAV("hit_body.mp3");
if(hit_body==NULL)
{
	printf("Failed ro load hit_body.mp3 ! Error:%s\n",Mix_GetError());
}
Snake_Body *snake_head=(Snake_Body*)malloc(sizeof(Snake_Body));
snake_head->snext=NULL;
snake_head->s_ele=(SDL_Rect){10*CELL_SIZE,10*CELL_SIZE,CELL_SIZE,CELL_SIZE};
Snake_Body *body2=(Snake_Body*)malloc(sizeof(Snake_Body));
Snake_Body *body3=(Snake_Body*)malloc(sizeof(Snake_Body));
snake_head->snext=body2;
body2->snext=body3;
body3->snext=NULL;
body2->s_ele=(SDL_Rect){9*CELL_SIZE,10*CELL_SIZE,CELL_SIZE,CELL_SIZE};
body3->s_ele=(SDL_Rect){8*CELL_SIZE,10*CELL_SIZE,CELL_SIZE,CELL_SIZE};

Food *food=(Food*)malloc(sizeof(Food));
food->F_ele=(SDL_Rect){4*CELL_SIZE,3*CELL_SIZE,CELL_SIZE,CELL_SIZE};


SDL_Rect wall_up={0,0,WIDTH,CELL_SIZE};
SDL_Rect wall_down={0,HEIGHT-CELL_SIZE,WIDTH,CELL_SIZE};
SDL_Rect wall_left={0,0,CELL_SIZE,HEIGHT};
SDL_Rect wall_right={WIDTH-CELL_SIZE,0,CELL_SIZE,HEIGHT};

bool alive=1;
SDL_Event event;
int score=0;
int dx=CELL_SIZE,dy=0;

if(bgm!=NULL)
{
	Mix_PlayMusic(bgm,-1);
}

while(alive)
{
	while(SDL_PollEvent(&event))
    		{
      			if(event.type==SDL_QUIT)
         		   {
           			alive=0;
         		   }

      			if(event.type==SDL_KEYDOWN)
   			   {
     				if(event.key.keysym.sym==SDLK_RIGHT)
        			   {dx=CELL_SIZE;dy=0;}
     				if(event.key.keysym.sym==SDLK_LEFT)
        			   {dx=-CELL_SIZE;dy=0;}
     				if(event.key.keysym.sym==SDLK_UP)
        			   {dy=-CELL_SIZE;dx=0;}
     				if(event.key.keysym.sym==SDLK_DOWN)
        			   {dy=CELL_SIZE;dx=0;}

  			   }

    		}


	int prev_x=snake_head->s_ele.x;
	int prev_y=snake_head->s_ele.y;
	snake_head->s_ele.x+=dx;
	snake_head->s_ele.y+=dy;

	Snake_Body *temp=snake_head->snext;
	while(temp!=NULL)
		{
		   int current_x=temp->s_ele.x;
		   int current_y=temp->s_ele.y;
		   temp->s_ele.x=prev_x;
		   temp->s_ele.y=prev_y;
	 	   prev_x=current_x;
		   prev_y=current_y;
		   temp=temp->snext;
		}

	int gameo=1;

	snake_head=if_eat_food(surface,snake_head,food);
	if(eat)
  	   score++;
	char scoreboard[100];
	sprintf(scoreboard,"Score: %d",score);
	gameo= if_snake_collide(snake_head,gameo);

	if(!gameo)
		{
		   alive=0;
		   if(gameover!=NULL)
  			{
			   Mix_PlayChannel(-1,gameover,0);
			   SDL_Delay(3000);
			}
		}
  	SDL_SetRenderDrawColor(surface,0,0,0,255);
  	SDL_RenderClear(surface);
	draw_Food(surface,food,snake_head);
	Snake_Body *draw=snake_head;
	while(draw!=NULL)
		{
		   DrawCurveEdgeCir(surface,draw->s_ele.x,draw->s_ele.y,CELL_SIZE,white);
		   draw=draw->snext;
		}
	show_boundary(surface,wall_up,wall_down,wall_left,wall_right);
 	//draw_grid(surface);
    	SDL_Surface *scoresurf=TTF_RenderText_Solid(font,scoreboard,black);
   	SDL_Texture *scoretext=SDL_CreateTextureFromSurface(surface,scoresurf);
   	SDL_Rect dst={20,0,scoresurf->w,scoresurf->h};
   	SDL_RenderCopy(surface,scoretext,NULL,&dst);
   	SDL_DestroyTexture(scoretext);
   	SDL_FreeSurface(scoresurf);
	eat=false;
	SDL_RenderPresent(surface);
   	SDL_Delay(300);
}

while(snake_head!=NULL)
	{
	   Snake_Body *next=snake_head->snext;
	   free(snake_head);
	   snake_head=next;
	}
SDL_DestroyRenderer(surface);
SDL_DestroyWindow(window);
clean_up_audio();
TTF_CloseFont(font);
TTF_Quit();
SDL_Quit();
return 0;
}

