#Paddle Game in SDL2

A simple paddle game built using C and SDL2.
The game includes paddle movement, ball physics,collision detection,and score handling.

## Features
-Smooth paddle movement
-Ball collision physics
-keyboard controls
-Boundary collisions
-SDL2 graphics rendering

##Technologies Used
-C Programming 
- SDL2 graphics rendering

## Controls
 |key|Action|
 |....|...|
 |Left Arrow|Move Left|
 |Right Arrow|Move Right|
 |ESC|Quit Game|
 |p|Pause Game|

 ##Installation 

 ###Linux (Ubuntu)

 install SDL2:
 ```bash
sudo apt install libsdl2-dev
```
#Compile
```bash
gcc paddlegame.c -o paddlegame `sdl2-config --cflags --libs`
```
##Run
```bash
./sdl_app
```







