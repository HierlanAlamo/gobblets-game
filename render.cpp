#include <SDL2/SDL.h>
#include <iostream>
#include "render.h"
#include <string.h>
#include "config.h"
#include "bot.h"
#include "board.h"
#include "playerEnum.h"


using namespace std;



bool RenderGame::init(SDL_Window*& gWindow, SDL_Surface*& gScreenSurface){
    bool success = true;

    // Inicializa SDL
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    // Cria janela
    gWindow = SDL_CreateWindow(
        "Gobblets Game :D",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    if(!gWindow){
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    gScreenSurface = SDL_GetWindowSurface(gWindow);

    return true;
}



bool RenderGame::loadMedia(SDL_Surface*& gSprite, std::string gameSprite){
    //Loading success flag
    bool success = true;

    //Load splash image
    gSprite = SDL_LoadBMP(gameSprite.c_str());
    if(gSprite == NULL){
        printf( "Unable to load image %s! SDL Error: %s\n", gameSprite.c_str(), SDL_GetError());
        success = false;
    }
    return success;
}

void RenderGame::close() {
    Object* allPieces[2][6] = {
        { &gSBluePiece, &gSBluePiece2, &gMBluePiece, &gMBluePiece2, &gBBluePiece, &gBBluePiece2 },
        { &gSRedPiece, &gSRedPiece2, &gMRedPiece, &gMRedPiece2, &gBRedPiece, &gBRedPiece2 }
    };

    for(int k = 0; k < 2; k++){
        for(int l = 0; l < 6; l++){
            if(allPieces[k][l]->sprite){
                SDL_FreeSurface(allPieces[k][l]->sprite);
                allPieces[k][l]->sprite = nullptr;
            }
        }
    }

    // Bot e game
    delete bot;
    bot = nullptr;
    delete game;
    game = nullptr;

    // Botões
    if(gPlayButton.sprite){ SDL_FreeSurface(gPlayButton.sprite); gPlayButton.sprite = nullptr; }
    if(gBotButton.sprite){ SDL_FreeSurface(gBotButton.sprite); gBotButton.sprite = nullptr; }
    if(g1V1Button.sprite){ SDL_FreeSurface(g1V1Button.sprite); g1V1Button.sprite = nullptr; }
    if(gEasyButton.sprite){ SDL_FreeSurface(gEasyButton.sprite); gEasyButton.sprite = nullptr; }
    if(gNormalButton.sprite){ SDL_FreeSurface(gNormalButton.sprite); gNormalButton.sprite = nullptr; }
    if(gHardButton.sprite){ SDL_FreeSurface(gHardButton.sprite); gHardButton.sprite = nullptr; }

    // Background
    if(gBackground.sprite){ SDL_FreeSurface(gBackground.sprite); gBackground.sprite = nullptr; }

    // Janela
    if(gWindow){
        SDL_DestroyWindow(gWindow);
        gWindow = nullptr;
    }

    // Finaliza SDL
    SDL_Quit();
}


RenderGame::RenderGame(){
    game = new Board();
    bot = new Bot(NORMAL);
    gBackground = createObject("game_sprites/emptyBackground.bmp");
    gWhoPlaysFirst = createObject("game_sprites/whoplaysfirst.bmp", 0, -0.25*SCREEN_HEIGHT, scale*1.5);
    gRedWins = createObject("game_sprites/redwins.bmp");
    gBlueWins = createObject("game_sprites/bluewins.bmp");
    gBotWins = createObject("game_sprites/botwins.bmp");
    // Buttons 
    gPlayButton = createObject("game_sprites/PlayButton.bmp", 0, 0.0*SCREEN_HEIGHT, scale);
    gBotButton = createObject("game_sprites/botbutton.bmp", 0, -0.075*SCREEN_HEIGHT, scale);
    g1V1Button = createObject("game_sprites/1v1button.bmp", 0, 0.075*SCREEN_HEIGHT, scale);
    gEasyButton = createObject("game_sprites/easybutton.bmp", 0, -0.15*SCREEN_HEIGHT, scale);
    gNormalButton = createObject("game_sprites/normalbutton.bmp", 0, 0.0*SCREEN_HEIGHT, scale);
    gHardButton = createObject("game_sprites/hardbutton.bmp", 0, 0.15*SCREEN_HEIGHT, scale);
    gRedButton = createObject("game_sprites/redbutton.bmp", 0, -0.075*SCREEN_HEIGHT, scale);
    gBlueButton = createObject("game_sprites/bluebutton.bmp", 0, 0.075*SCREEN_HEIGHT, scale);
    // Pieces
    gSBluePiece = createObject("game_sprites/s_blue_piece.bmp", -200, 210, scale);
    gSBluePiece2 = createObject("game_sprites/s_blue_piece.bmp", -150, 210, scale);
    gMBluePiece = createObject("game_sprites/m_blue_piece.bmp", -100, 210, scale);
    gMBluePiece2 = createObject("game_sprites/m_blue_piece.bmp", -50, 210, scale);
    gBBluePiece = createObject("game_sprites/b_blue_piece.bmp", 0, 210, scale);
    gBBluePiece2 = createObject("game_sprites/b_blue_piece.bmp", 50, 210, scale);
    gSRedPiece = createObject("game_sprites/s_red_piece.bmp", -200, 140, scale);
    gSRedPiece2 = createObject("game_sprites/s_red_piece.bmp", -150, 140, scale);
    gMRedPiece = createObject("game_sprites/m_red_piece.bmp", -100, 140, scale);
    gMRedPiece2 = createObject("game_sprites/m_red_piece.bmp", -50, 140, scale);
    gBRedPiece = createObject("game_sprites/b_red_piece.bmp", 0, 140, scale);
    gBRedPiece2 = createObject("game_sprites/b_red_piece.bmp", 50, 140, scale);
    // Surfaces
    gScreenSurface = NULL;
    gWindow = NULL;
    pieces[0][0] = &gSBluePiece;
    pieces[0][1] = &gSBluePiece2;
    pieces[0][2] = &gMBluePiece;
    pieces[0][3] = &gMBluePiece2;
    pieces[0][4] = &gBBluePiece;
    pieces[0][5] = &gBBluePiece2;
    pieces[1][0] = &gSRedPiece;
    pieces[1][1] = &gSRedPiece2;
    pieces[1][2] = &gMRedPiece;
    pieces[1][3] = &gMRedPiece2;
    pieces[1][4] = &gBRedPiece;
    pieces[1][5] = &gBRedPiece2;
    // Defining grid positions
    for(int i=0;i<DIMENSIONS;i++){
        for(int j=0;j<DIMENSIONS;j++){
            grid[i][j] = createGrid(pos[i][j][0],pos[i][j][1]);
        }
    }
}

void RenderGame::checkBoardAndPlace(){
    Object* piece;
    int isPlaced[3] = {2,2,2};

    for(int p = 0; p < 6; p++){
        pieces[PLAYER_2-1][p]->isPlaced = false;
    }

    for(int p = 0; p < 6; p++){
        if(pieces[PLAYER_2-1][p]->isPlaced == true){
            pieces[PLAYER_2-1][p]->rect.x = -1000;
            pieces[PLAYER_2-1][p]->rect.y = -1000;
        }
    }

    for(int i=0;i<DIMENSIONS;i++){
        for(int j=0;j<DIMENSIONS;j++){
            Player player_ = game->getPlaceHolder(i,j);
            Size size_ = game->getPieceSize(i,j);
            if(game->getPlaceHolder(i,j) == PLAYER_2){
                if(isPlaced[size_-1] == 2){
                    piece = pieces[player_-1][(size_-1)*2];
                    piece->isPlaced = true;
                    isPlaced[size_-1] -=1;
                    piece->rect.x = pos[i][j][0] - piece->rect.w/2;
                    piece->rect.y = pos[i][j][1] - piece->rect.h/2;
                }else if(isPlaced[size_-1] == 1){
                    piece = pieces[player_-1][(size_-1)*2+1];
                    piece->isPlaced = true;
                    isPlaced[size_-1] -=1;
                    piece->rect.x = pos[i][j][0] - piece->rect.w/2;
                    piece->rect.y = pos[i][j][1] - piece->rect.h/2;
                }  
            }
        }
    }
}

void RenderGame::PlaceInGrid(){
    for(int k=0;k<2;k++){
        for(int l=0;l<6;l++){
            for(int i=0;i<DIMENSIONS;i++){
                for(int j=0;j<DIMENSIONS;j++){
                    if(SDL_HasIntersection(&pieces[k][l]->rect, &grid[i][j])){
                        if(!wasAPiecePlaced(static_cast<Player>(k+1),static_cast<Size>((l+2)/2))){
                            MovePieceInGrid(refPieces[k][l], pieces[k][l], i, j);
                        }else{
                            placePiece(i,j,static_cast<Size>((l+2)/2),static_cast<Player>(k+1),pieces[k][l]);
                        }
                    }
                }
            }
        }
    }
}

bool RenderGame::wasAPiecePlaced(Player player, Size size){
    int playerIndex = player - 1;
    int baseIndex = (size - 1) * 2;
    int countNow = 0;
    int countBefore = 0;

    for(int l = baseIndex; l < baseIndex + 2; l++){
        for(int i = 0; i < DIMENSIONS; i++){
            for(int j = 0; j < DIMENSIONS; j++){
                if(SDL_HasIntersection(&pieces[playerIndex][l]->rect, &grid[i][j])){
                    countNow++;
                    goto nextNow;
                }
            }
        }
        nextNow:;
    }

    for(int l = baseIndex; l < baseIndex + 2; l++){
        for(int i = 0; i < DIMENSIONS; i++){
            for(int j = 0; j < DIMENSIONS; j++){
                if(SDL_HasIntersection(&refPieces[playerIndex][l].rect, &grid[i][j])){
                    countBefore++;
                    goto nextBefore;
                }
            }
        }
        nextBefore:;
    }
    return countNow != countBefore;
}



void RenderGame::updateReferencePieces(){
    for(int k=0;k<2;k++){
        for(int l=0;l<6;l++){
            refPieces[k][l] = *pieces[k][l];
        }
    }
}

bool RenderGame::gameChanged() {
    for(int k = 0; k < 2; k++){
        for(int l = 0; l < 6; l++){
            if(!(refPieces[k][l] == *pieces[k][l])){ 
                return true;
            }
        }
    }
    return false;
}


void RenderGame::placePiece(int line, int col, Size size, Player actualPlayer,Object* piece){
    Board oldgame = *game;
    if(game->getNbPiecesInHouse(actualPlayer,size) != 0 && oldgame.placePiece(actualPlayer, size, line+1, col+1) == 0){
        piece->rect.x = pos[line][col][0] - piece->rect.w/2;
        piece->rect.y = pos[line][col][1] - piece->rect.h/2;
        piece->init_x = pos[line][col][0] - piece->rect.w/2;;
        piece->init_y = pos[line][col][1] - piece->rect.h/2;;

        if(game->getNbPiecesInHouse(actualPlayer,size) == 1){
            piece->id = 2;
        }else if(game->getNbPiecesInHouse(actualPlayer,size) == 2){
            piece->id = 1;
        }

        if(actualPlayer == PLAYER_1){
            game->placePiece(actualPlayer, size, line+1, col+1);
            piece->size = game->getPieceSize(line, col);
        }else if(actualPlayer == PLAYER_2 && isBotON == false){
            game->placePiece(actualPlayer, size, line+1, col+1);
            piece->size = game->getPieceSize(line, col);
        }
    }
}

void RenderGame::MovePieceInGrid(Object refPiece, Object* piece, int line, int col){
    Board oldgame = *game;
    for(int i=0;i<DIMENSIONS;i++){
        for(int j=0;j<DIMENSIONS;j++){    
            if(SDL_HasIntersection(&refPiece.rect, &grid[i][j])){
                if(oldgame.movePiece(i+1,j+1,line+1,col+1) == 0){
                    game->movePiece(i+1,j+1,line+1,col+1);
                    piece->rect.x = pos[line][col][0] - piece->rect.w/2;
                    piece->rect.y = pos[line][col][1] - piece->rect.h/2;
                    piece->init_x = pos[line][col][0] - piece->rect.w/2;;
                    piece->init_y = pos[line][col][1] - piece->rect.h/2;;
                }
            }
        }
    }
}

Object RenderGame::createObject(std::string gameSprite, int pos_x, int pos_y, int scale){
    Object newObject;
    if(!loadMedia(newObject.sprite, gameSprite)){
        printf("Failed to load media! %s\n", gameSprite.c_str());
    }
    if(scale != 0) newObject.rect = createBody(newObject.sprite, pos_x, pos_y, scale);
    else newObject.rect = createBackgroud();
    newObject.init_x = (SCREEN_WIDTH - (newObject.sprite->w * scale)) / 2 + pos_x;
    newObject.init_y = (SCREEN_HEIGHT - (newObject.sprite->h * scale)) / 2 + pos_y;
    return newObject;
}

SDL_Rect RenderGame::createBody(SDL_Surface*& gObject, int pos_x, int pos_y, int scale){
    SDL_Rect newBody;
    newBody.x = (SCREEN_WIDTH-(gObject->w*scale))/2 + pos_x;
    newBody.y = (SCREEN_HEIGHT-(gObject->h*scale))/2 + pos_y;
    newBody.w = gObject->w*scale; 
    newBody.h = gObject->h*scale; 
    return newBody;
}

SDL_Rect RenderGame::createGrid(int pos_x, int pos_y){
    SDL_Rect newBody;
    newBody.x = pos_x - 25;
    newBody.y = pos_y - 40;
    newBody.w = 50;
    newBody.h = 50; 
    return newBody;
}

SDL_Rect RenderGame::createBackgroud(){
    SDL_Rect newBackgroud;
    newBackgroud.x = 0;
    newBackgroud.y = 0;
    newBackgroud.w = SCREEN_WIDTH; 
    newBackgroud.h = SCREEN_HEIGHT;
    return newBackgroud;
}

void RenderGame::resetAllPiecesOutsideGrid() {
    for(int k = 0; k < 2; k++){
        for(int l = 0; l < 6; l++){
            Object* piece = pieces[k][l];
            bool insideGrid = false;
            for(int i = 0; i < 3; i++){
                for(int j = 0; j < 3; j++){
                    int centerX = pos[i][j][0];
                    int centerY = pos[i][j][1];
                    if(piece->rect.x + piece->rect.w / 2 == centerX &&
                        piece->rect.y + piece->rect.h / 2 == centerY){
                        insideGrid = true;
                        break;
                    }
                }
                if(insideGrid) break;
            }
            if(!insideGrid){
                piece->rect.x = piece->init_x;
                piece->rect.y = piece->init_y;
            }
        }
    }
}