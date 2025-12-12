#ifndef RENDER_H
#define RENDER_H

#include "object.h"
#include "board.h"
#include "bot.h"

class RenderGame
{
    private:
        int scale = 2;
    public:
        bool isBotON = false;
        RenderGame();
        int pos[3][3][2] =  {{{181,175},{255,175},{327,175}},
                             {{181,248},{255,248},{327,248}},
                             {{181,322},{255,322},{327,322}}};
        Bot* bot;
        Board* game;
        Player actualPlayer;
        SDL_Surface* gScreenSurface;
        SDL_Window* gWindow;
        Object gBackground, gWhoPlaysFirst;
        Object gRedWins, gBlueWins, gBotWins;
        Object gPlayButton, gBotButton, g1V1Button;
        Object gEasyButton, gNormalButton, gHardButton;
        Object gBlueButton, gRedButton;
        Object gSBluePiece, gSBluePiece2;
        Object gMBluePiece, gMBluePiece2;
        Object gBBluePiece, gBBluePiece2;
        Object gSRedPiece, gSRedPiece2;
        Object gMRedPiece, gMRedPiece2;
        Object gBRedPiece, gBRedPiece2;
        Object* pieces[2][6];
        Object refPieces[2][6];
        SDL_Rect grid[3][3];
        void updateReferencePieces();
        void resetAllPiecesOutsideGrid();
        void MovePieceInGrid(Object refPiece, Object* piece, int line, int col);
        void placePiece(int line, int col, Size size, Player actualPlayer,Object* piece = NULL);
        void PlaceInGrid();
        void checkBoardAndPlace();
        bool gameChanged();
        bool wasAPiecePlaced(Player player, Size size);
        SDL_Rect createBody(SDL_Surface*& gObject, int pos_x, int pos_y, int scale = 1);
        SDL_Rect createGrid(int pos_x, int pos_y);
        SDL_Rect createBackgroud();
        Object createObject(std::string gameSprite, int pos_x = 0, int pos_y = 0, int scale = 0);
        SDL_Point mousePos;
        // Stop and start functions
        void close();
        bool loadMedia(SDL_Surface*& gBackground, std::string gameSprite);
        bool init(SDL_Window*& gWindow, SDL_Surface*& gScreenSurface);
};

#endif