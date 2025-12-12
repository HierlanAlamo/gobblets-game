#include "object.h"

using namespace std;

Object::Object()
{
    // Nothing :)
}

bool Object::moveBody(const SDL_Event& e, Object* pieces[2][6]) {
    int thisIndex = -1;
    if(e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT){
        int mx = e.button.x;
        int my = e.button.y;

        if(mx >= rect.x && mx <= rect.x + rect.w &&
            my >= rect.y && my <= rect.y + rect.h){

            for(int k = 0; k < 2; k++){
                for(int l = 0; l < 6; l++){
                    if(pieces[k][l] == this){
                        thisIndex = l;
                        break;
                    }
                }
                if (thisIndex != -1) break;
            }
            for(int l = 5; l >= thisIndex; l--){
                Object* other = pieces[1][l];
                if (other == this) break; 
                if (SDL_HasIntersection(&rect, &other->rect)) {
                    return false;
                }
            }
            for(int l = 5; l >= thisIndex; l--){ 
                Object* other = pieces[0][l];
                if(other == this) break;  
                if(SDL_HasIntersection(&rect, &other->rect)){
                    return false;
                }
            }
            dragging = true;
            offsetX = mx - rect.x;
            offsetY = my - rect.y;
            return true;
        }

        return false;
    }

    if(e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_LEFT){
        if (dragging){
            dragging = false;
            return true;
        }
        return false;
    }

    if(e.type == SDL_MOUSEMOTION && dragging){
        if((e.motion.x - offsetX) > 0 && (e.motion.x - offsetX) + rect.w < SCREEN_WIDTH)
            rect.x = e.motion.x - offsetX;

        if((e.motion.y - offsetY) > 0 && (e.motion.y - offsetY) + rect.h < SCREEN_HEIGHT)
            rect.y = e.motion.y - offsetY;

        return true;
    }

    return false;
}


void Object::renderObject(SDL_Surface* gScreenSurface){
    SDL_BlitScaled(sprite, NULL, gScreenSurface, &rect);
}

bool operator==(const Object& a, const Object& b){
    return a.rect.x == b.rect.x &&
           a.rect.y == b.rect.y;
}