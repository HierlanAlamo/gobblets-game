#ifndef OBJECT_H
#define OBJECT_H

#include <SDL2/SDL.h>
#include <iostream>
#include <string.h>
#include "piece.h"

const int SCREEN_WIDTH = 512;
const int SCREEN_HEIGHT = 512;

using namespace std;

class Object
{
private:
    bool dragging = false;
    int offsetX = 0, offsetY = 0;
public:
    int id = 0;
    int init_x, init_y;
    Size size;
    void renderObject(SDL_Surface* gScreenSurface);
    SDL_Surface* sprite = NULL;
    SDL_Rect rect;
    bool moveBody(const SDL_Event& e, Object* pieces[2][6]);
    Object();
    friend bool operator==(const Object& a, const Object& b);
    //~Object();

};




#endif