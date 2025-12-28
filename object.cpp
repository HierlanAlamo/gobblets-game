#include "object.h"

using namespace std;

Object::Object()
{
    // Inicialize com valores padrão
    originalR = 255;
    originalG = 255;
    originalB = 255;
    dragging = false;
    startX = 0;
    startY = 0;
}

bool Object::moveBody(const SDL_Event& e, Object* pieces[2][6]) {
    int thisIndex = -1;
    if(e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT){
        int mx = e.button.x;
        int my = e.button.y;
        if(mx >= rect.x && mx <= rect.x + rect.w &&
            my >= rect.y && my <= rect.y + rect.h){
            SDL_SetSurfaceColorMod(sprite, 100, 100, 100);
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
            startX = mx - rect.x;
            startY = my - rect.y;
            return true;
        }
        return false;
    }

    if(e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_LEFT){
        if (dragging){
            rect.x = e.button.x - startX;
            rect.y = e.button.y - startY;
            SDL_SetSurfaceColorMod(sprite, originalR, originalG, originalB);
            dragging = false;
            return true;
        }
        SDL_SetSurfaceColorMod(sprite, originalR, originalG, originalB);
        return false;
    }

    if(e.type == SDL_MOUSEMOTION && dragging){
        return true;
    }

    return false;
    SDL_SetSurfaceColorMod(sprite, originalR, originalG, originalB);
}


void Object::renderObject(SDL_Surface* gScreenSurface){
    SDL_BlitScaled(sprite, NULL, gScreenSurface, &rect);
}

bool operator==(const Object& a, const Object& b){
    return a.rect.x == b.rect.x &&
           a.rect.y == b.rect.y;
}

void Object::resetDragState(){
    if(dragging){
        SDL_SetSurfaceColorMod(sprite, originalR, originalG, originalB);
        dragging = false;
    }
}