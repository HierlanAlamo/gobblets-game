#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include "render.h"
#include <string.h>

using namespace std;

int main(int argc, char* args[]){
    int winner_countp1 = 0;
    int winner_countp2 = 0;
    int notThree = 0;
    Player oldWinner = NO_PLAYER;
    // Inicializing SDL
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0){
        cerr << "Failed to initialize SDL: " << SDL_GetError() << endl;
        return 1;
    }

    // Inicializing SDL_mixer
    bool audioOK = true;
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0){
        cout << "[WARNING] SDL_mixer couldn't (container without audio). Keep going without audio.\n";
        audioOK = false;
    }

    // Loading music (if it works)
    Mix_Music* music = nullptr;
    if(audioOK){
        music = Mix_LoadMUS("game_songs/songGameIMT.wav");
        if(!music){
            cout << "[AVISO] ERROR when trying to load audio. Keep going without audio.\n";
            audioOK = false;
        }else{
            Mix_PlayMusic(music, -1); // music loop
        }
    }


    bool quit = false, pieceBeingDragged = false;
    int currentFrame = 0;
    Uint32 lastUpdate = 0;
    const int frameWidth = 25, frameHeight = 25;
    const int cols = 47, rows = 1;
    const int totalFrames = cols * rows;

    
    RenderGame* render = new RenderGame();
    int screenNum = 0;
    bool newBackground = false;
    if(!render->init(render->gWindow, render->gScreenSurface)){
        printf("Failed to initialize!\n");
    }
    else{
        // Starting Animation
        SDL_Surface* temp = SDL_LoadBMP("game_sprites/cat_noir.bmp");
        if(!temp){
            printf("Failed to load cat_noir.bmp: %s\n", SDL_GetError());
            return 1;
        }

        SDL_Surface* catSheet = SDL_ConvertSurface(temp, render->gScreenSurface->format, 0);
        SDL_FreeSurface(temp);

        SDL_SetColorKey(catSheet, SDL_TRUE, SDL_MapRGB(catSheet->format, 0, 0, 0));

        SDL_UpdateWindowSurface(render->gWindow);
        SDL_Event e;
        while(!quit && winner_countp1 < 2 && winner_countp2 < 2){
            // Next frame animation
            if (SDL_GetTicks() - lastUpdate > 120) { // 120ms per frame
                currentFrame++;
                currentFrame %= totalFrames;
                lastUpdate = SDL_GetTicks();
            }

            while(SDL_PollEvent(&e)){
                if(e.type == SDL_QUIT) quit = true;
                if(e.type == SDL_MOUSEBUTTONDOWN){
                    if(e.button.button == SDL_BUTTON_LEFT){
                        render->mousePos = {e.button.x, e.button.y};
                        if(SDL_PointInRect(&render->mousePos, &render->gPlayButton.rect) && screenNum == 0){
                            SDL_FreeSurface(render->gPlayButton.sprite);
                            render->gPlayButton.sprite = NULL;
                            screenNum = 1;
                        }
                        else if(SDL_PointInRect(&render->mousePos, &render->gBotButton.rect) && screenNum == 1){
                            SDL_FreeSurface(render->gBotButton.sprite);
                            render->gBotButton.sprite = NULL;
                            screenNum = 2;
                            render->isBotON = true;
                        }
                        else if(SDL_PointInRect(&render->mousePos, &render->g1V1Button.rect) && screenNum == 1){
                            SDL_FreeSurface(render->g1V1Button.sprite);
                            render->g1V1Button.sprite = NULL;
                            screenNum = 3;
                        }else if(SDL_PointInRect(&render->mousePos, &render->gEasyButton.rect) && screenNum == 2){
                            SDL_FreeSurface(render->gEasyButton.sprite);
                            render->gEasyButton.sprite = NULL;
                            render->bot->selectMode(EASY);
                            render->actualPlayer = PLAYER_1;
                            screenNum = 4;
                        }else if(SDL_PointInRect(&render->mousePos, &render->gNormalButton.rect) && screenNum == 2){
                            SDL_FreeSurface(render->gNormalButton.sprite);
                            render->gNormalButton.sprite = NULL;
                            render->bot->selectMode(NORMAL);
                            render->actualPlayer = PLAYER_1;
                            screenNum = 4;
                        }else if(SDL_PointInRect(&render->mousePos, &render->gHardButton.rect) && screenNum == 2){
                            SDL_FreeSurface(render->gHardButton.sprite);
                            render->gHardButton.sprite = NULL;
                            render->bot->selectMode(HARD);
                            render->actualPlayer = PLAYER_1;
                            screenNum = 4;
                        }else if(SDL_PointInRect(&render->mousePos, &render->gBlueButton.rect) && screenNum == 3){
                            SDL_FreeSurface(render->gBlueButton.sprite);
                            render->gBlueButton.sprite = NULL;
                            SDL_FreeSurface(render->gRedButton.sprite);
                            render->gRedButton.sprite = NULL;
                            SDL_FreeSurface(render->gWhoPlaysFirst.sprite);
                            render->gWhoPlaysFirst.sprite = NULL;
                            render->actualPlayer = PLAYER_1;
                            screenNum = 4;
                        }else if(SDL_PointInRect(&render->mousePos, &render->gRedButton.rect) && screenNum == 3){
                            SDL_FreeSurface(render->gRedButton.sprite);
                            render->gRedButton.sprite = NULL;
                            SDL_FreeSurface(render->gRedButton.sprite);
                            render->gRedButton.sprite = NULL;
                            SDL_FreeSurface(render->gWhoPlaysFirst.sprite);
                            render->gWhoPlaysFirst.sprite = NULL;
                            render->actualPlayer = PLAYER_2;
                            screenNum = 4;
                            
                        }
                    }
                }

                if(render->actualPlayer == PLAYER_1){
                    if(!pieceBeingDragged) pieceBeingDragged = render->gBBluePiece2.moveBody(e, render->pieces);
                    if(!pieceBeingDragged) pieceBeingDragged = render->gBBluePiece.moveBody(e, render->pieces);
                    if(!pieceBeingDragged) pieceBeingDragged = render->gMBluePiece2.moveBody(e, render->pieces);
                    if(!pieceBeingDragged) pieceBeingDragged = render->gMBluePiece.moveBody(e, render->pieces);
                    if(!pieceBeingDragged) pieceBeingDragged = render->gSBluePiece.moveBody(e, render->pieces);
                    if(!pieceBeingDragged) pieceBeingDragged = render->gSBluePiece2.moveBody(e, render->pieces);
                }

                if(!render->isBotON && render->actualPlayer == PLAYER_2){
                    if(!pieceBeingDragged) pieceBeingDragged = render->gBRedPiece2.moveBody(e, render->pieces);
                    if(!pieceBeingDragged) pieceBeingDragged = render->gBRedPiece.moveBody(e, render->pieces);
                    if(!pieceBeingDragged) pieceBeingDragged = render->gMRedPiece2.moveBody(e, render->pieces);
                    if(!pieceBeingDragged) pieceBeingDragged = render->gMRedPiece.moveBody(e, render->pieces);
                    if(!pieceBeingDragged) pieceBeingDragged = render->gSRedPiece2.moveBody(e, render->pieces);
                    if(!pieceBeingDragged) pieceBeingDragged = render->gSRedPiece.moveBody(e, render->pieces);
                }

                if(e.type == SDL_MOUSEBUTTONUP){
                    render->PlaceInGrid();
                    render->resetAllPiecesOutsideGrid();
                    if(render->gameChanged() && winner_countp1 < 2 && winner_countp2 < 2){
                        if(render->isBotON){
                            if(render->game->getWinner() == PLAYER_2 && oldWinner == NO_PLAYER) winner_countp2=2;
                            if(render->game->getWinner() != NO_PLAYER && notThree < 2){
                                if(render->game->getWinner() == PLAYER_1 && oldWinner == PLAYER_1) winner_countp1++;
                                if(render->game->getWinner() == PLAYER_2 && oldWinner == PLAYER_2) winner_countp2++;
                                if(render->game->getWinner() == PLAYER_1) winner_countp1++;
                                if(render->game->getWinner() == PLAYER_2) winner_countp2++;
                                notThree++;
                            }else{
                                winner_countp1 = 0;
                                winner_countp2 = 0;
                                notThree = 0;
                            }
                            oldWinner = render->game->getWinner();
                            if(winner_countp1 < 2 && winner_countp2 < 2){
                                render->bot->botTurn(*render->game);
                                render->checkBoardAndPlace();
                                if(render->game->getWinner() == PLAYER_1 && oldWinner == NO_PLAYER) winner_countp1=2;
                                if(render->game->getWinner() != NO_PLAYER && notThree < 2){
                                    if(render->game->getWinner() == PLAYER_1 && oldWinner == PLAYER_1) winner_countp1++;
                                    if(render->game->getWinner() == PLAYER_2 && oldWinner == PLAYER_2) winner_countp2++;
                                    if(render->game->getWinner() == PLAYER_1) winner_countp1++;
                                    if(render->game->getWinner() == PLAYER_2) winner_countp2++;
                                    notThree++;
                                }else{
                                    winner_countp1 = 0;
                                    winner_countp2 = 0;
                                    notThree = 0;
                                }
                                oldWinner = render->game->getWinner();
                            }
                        }else{
                            render->actualPlayer = render->game->nextPlayer(render->actualPlayer);
                            if(render->game->getWinner() == PLAYER_1 && render->actualPlayer == PLAYER_1 && oldWinner == NO_PLAYER) winner_countp1=2;
                            if(render->game->getWinner() == PLAYER_2 && render->actualPlayer == PLAYER_2 && oldWinner == NO_PLAYER) winner_countp2=2;
                            if(render->game->getWinner() != NO_PLAYER && notThree < 2){
                                if(render->game->getWinner() == PLAYER_1 && oldWinner == PLAYER_1) winner_countp1++;
                                if(render->game->getWinner() == PLAYER_2 && oldWinner == PLAYER_2) winner_countp2++;
                                if(render->game->getWinner() == PLAYER_1) winner_countp1++;
                                if(render->game->getWinner() == PLAYER_2) winner_countp2++;
                                notThree++;
                            }else{
                                winner_countp1 = 0;
                                winner_countp2 = 0;
                                notThree = 0;
                            }
                            oldWinner = render->game->getWinner();
                        }
                        render->updateReferencePieces();
                        
                    }
                    std::cout << *render->game;
                }
                pieceBeingDragged = false;

                
                
            }
            // it changes the background sprite
            if(screenNum == 4 && !newBackground){
                if(render->gBackground.sprite != nullptr){
                    SDL_FreeSurface(render->gBackground.sprite);
                    render->gBackground.sprite = nullptr;
                }
                render->gBackground = render->createObject("game_sprites/background.bmp");
                newBackground = true;
            }

            render->gBackground.renderObject(render->gScreenSurface);
            if(screenNum == 0){
                render->gPlayButton.renderObject(render->gScreenSurface);
            }else if(screenNum == 1){
                render->gBotButton.renderObject(render->gScreenSurface);
                render->g1V1Button.renderObject(render->gScreenSurface);
            }else if(screenNum == 2){
                render->gEasyButton.renderObject(render->gScreenSurface);
                render->gNormalButton.renderObject(render->gScreenSurface);
                render->gHardButton.renderObject(render->gScreenSurface);
            }else if(screenNum == 3){
                render->gBlueButton.renderObject(render->gScreenSurface);
                render->gRedButton.renderObject(render->gScreenSurface);
                render->gWhoPlaysFirst.renderObject(render->gScreenSurface);
            }
            
            // Animation
            if(screenNum == 4){
                int col = currentFrame % cols;
                int row = currentFrame / cols;
                SDL_Rect src = { col * frameWidth, row * frameHeight, frameWidth, frameHeight };
                SDL_Rect dst = { 232, 100, frameWidth * 2, frameHeight * 2 }; // dobra a escala
                SDL_BlitScaled(catSheet, &src, render->gScreenSurface, &dst);
            }
            if(screenNum == 4){
                render->gSBluePiece.renderObject(render->gScreenSurface);
                render->gSRedPiece.renderObject(render->gScreenSurface);
                render->gSBluePiece2.renderObject(render->gScreenSurface);
                render->gSRedPiece2.renderObject(render->gScreenSurface);
                render->gMBluePiece.renderObject(render->gScreenSurface);
                render->gMRedPiece.renderObject(render->gScreenSurface);
                render->gMBluePiece2.renderObject(render->gScreenSurface);
                render->gMRedPiece2.renderObject(render->gScreenSurface);
                render->gBBluePiece.renderObject(render->gScreenSurface);
                render->gBRedPiece.renderObject(render->gScreenSurface);
                render->gBBluePiece2.renderObject(render->gScreenSurface);
                render->gBRedPiece2.renderObject(render->gScreenSurface);
            }
            SDL_UpdateWindowSurface(render->gWindow);
        }
        SDL_FreeSurface(catSheet);
    }
    if(render->game->getWinner() == PLAYER_1){
        render->gBlueWins.renderObject(render->gScreenSurface);
    }else if(render->game->getWinner() == PLAYER_2 && render->isBotON == false){
        render->gRedWins.renderObject(render->gScreenSurface);
    }else if(render->game->getWinner() == PLAYER_2 && render->isBotON == true){
        render->gBotWins.renderObject(render->gScreenSurface);
    }else{
        std::cout << "Error";
    }

    SDL_UpdateWindowSurface(render->gWindow);
    SDL_Delay(5000);
    render->close();
    delete render;

    // Cleaning
    Mix_FreeMusic(music);
    Mix_CloseAudio();
    SDL_Quit();

    return 0;
}