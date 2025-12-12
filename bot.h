#ifndef BOT_H
#define BOT_H

#include <iostream>
#include <string>
#include "board.h"
#include "piece.h"
#include "config.h"
#include "cell.h"

using namespace std;

enum Mode{
    UNSELECT,
    EASY,
    NORMAL,
    HARD
};

class Bot
{
    private:
        Mode gameMode;
        Size randSize();
        int randLC();
        int randMove();
    public:
        Bot();
        Bot(Mode gameMode);
        StatusEnum botTurn(Board& board);
        void selectMode(Mode gameMode);
};

#endif