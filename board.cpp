#include <iostream>
#include <string>
#include "board.h"
#include "piece.h"
#include "config.h"
#include "cell.h"
#include "playerEnum.h"
#include "statusEnum.h"

using namespace std;

Board::Board(){
    for(int i = 0; i < DIMENSIONS; i++){
        for(int j = 0; j < DIMENSIONS; j++){
            this->state[i][j] = Cell();
        }
    }
    for(int i=0; i< NB_SIZE; i++){
        this->p1Pieces[i] = 2;
        this->p2Pieces[i] = 2;
    }
}

Board::~Board() {
    // Nothing Here :)
}


Board::Board(const Board& other){
    for(int i = 0; i < DIMENSIONS; i++){
        for(int j = 0; j < DIMENSIONS; j++){
            this->state[i][j] = other.state[i][j];
        }
    }
    for(int i=0; i< NB_SIZE; i++){
        this->p1Pieces[i] = other.p1Pieces[i];
        this->p2Pieces[i] = other.p2Pieces[i];
    }
}

Cell& Board::getCellAt(int line, int column){
    return this->state[line][column];
}

int* Board::getHousesOfPlayer(Player player) {
    if(player == PLAYER_1){
        return p1Pieces;
    }else{
        return p2Pieces;
    } 
}

Player Board::nextPlayer(Player currentPlayer) const{
    if(currentPlayer == PLAYER_1){
        return PLAYER_2;
    }else{
        return PLAYER_1;
    }
}

Player Board::getPlaceHolder(int line, int column){
    if(!this->state[line][column].canPop()) return NO_PLAYER;
    return state[line][column].peek().getOwner();
}

Size Board::getPieceSize(int line, int column){
    if(!this->state[line][column].canPop()) return NONE;
    return state[line][column].peek().getSize();
}

int Board::getNbPiecesInHouse(Player checkedPlayer, Size pieceSize){
    if(checkedPlayer == PLAYER_1){
        return this->p1Pieces[pieceSize-1];
    }else if(checkedPlayer == PLAYER_2){
        return this->p2Pieces[pieceSize-1];
    }
    return -1;
}

StatusEnum Board::placePiece(Player currentPlayer, Size pieceSize, int line, int column){
    Board next_mov = Board(*this);
    // Player currentWinner = this->getWinner();
    if(line>3 || line < 1 || column>3 || column < 1 || currentPlayer == NO_PLAYER || pieceSize == NONE){
        return INVALID_ENTRY;
    }else if(getPieceSize(line-1,column-1) == NONE){ // this->state[line-1][column-1].canPush(Piece(currentPlayer, pieceSize))
        if(currentPlayer == PLAYER_1 && this->p1Pieces[pieceSize-1]!=0){
            if(getWinner() != PLAYER_2){
                this->state[line-1][column-1].push(Piece(currentPlayer, pieceSize));
                this->p1Pieces[pieceSize-1]--;
            }else{
                return INVALID_ENTRY;
            }
        }else if(currentPlayer == PLAYER_2 && this->p2Pieces[pieceSize-1]!=0){
            if(getWinner() != PLAYER_1){
                this->state[line-1][column-1].push(Piece(currentPlayer, pieceSize));
                this->p2Pieces[pieceSize-1]--;
            }else{
                return INVALID_ENTRY;
            }
        }else{
            return INVALID_ENTRY;
        }
        return OK;
    }else if(getPieceSize(line-1, column-1) != NONE){
        next_mov.state[line-1][column-1].push(Piece(currentPlayer, pieceSize));
        if(currentPlayer == PLAYER_1 && this->p1Pieces[pieceSize-1]!=0){
            if(pieceSize == LARGE && getWinner() == PLAYER_2 && getPlaceHolder(line-1,column-1) == PLAYER_2 && next_mov.getWinner() != PLAYER_2){
                this->state[line-1][column-1].push(Piece(currentPlayer, pieceSize));
                this->p1Pieces[pieceSize-1]--;
            }else if(pieceSize == MEDIUM && getWinner() == PLAYER_2 && this->state[line-1][column-1].canPush(Piece(currentPlayer, pieceSize)) && getPlaceHolder(line-1,column-1) == PLAYER_2 && next_mov.getWinner() != PLAYER_2){
                this->state[line-1][column-1].push(Piece(currentPlayer, pieceSize));
                this->p1Pieces[pieceSize-1]--;
            }
            // else if(pieceSize == LARGE && getPlaceHolder(line-1,column-1) == PLAYER_2 && next_mov.getWinner() != NO_PLAYER){
            //     this->state[line-1][column-1].push(Piece(currentPlayer, pieceSize));
            //     this->p1Pieces[pieceSize-1]--;
            // }else if(pieceSize == MEDIUM && this->state[line-1][column-1].canPush(Piece(currentPlayer, pieceSize)) && getPlaceHolder(line-1,column-1) == PLAYER_2 && next_mov.getWinner() != NO_PLAYER){
            //     this->state[line-1][column-1].push(Piece(currentPlayer, pieceSize));
            //     this->p1Pieces[pieceSize-1]--;
            // }
            else{
                return INVALID_ENTRY;
            }
        }else if(currentPlayer == PLAYER_2 && this->p2Pieces[pieceSize-1]!=0 ){
            if(pieceSize == LARGE && getWinner() == PLAYER_1 && getPlaceHolder(line-1,column-1) == PLAYER_1 && next_mov.getWinner() != PLAYER_1){
                this->state[line-1][column-1].push(Piece(currentPlayer, pieceSize));
                this->p2Pieces[pieceSize-1]--;
            }else if(pieceSize == MEDIUM && getWinner() == PLAYER_1 && this->state[line-1][column-1].canPush(Piece(currentPlayer, pieceSize)) && getPlaceHolder(line-1,column-1) == PLAYER_1 && next_mov.getWinner() != PLAYER_1){
                this->state[line-1][column-1].push(Piece(currentPlayer, pieceSize));
                this->p2Pieces[pieceSize-1]--;
            }
            // else if(pieceSize == LARGE && getPlaceHolder(line-1,column-1) == PLAYER_1 && next_mov.getWinner() != NO_PLAYER){
            //     this->state[line-1][column-1].push(Piece(currentPlayer, pieceSize));
            //     this->p2Pieces[pieceSize-1]--;
            // }else if(pieceSize == MEDIUM && this->state[line-1][column-1].canPush(Piece(currentPlayer, pieceSize)) && getPlaceHolder(line-1,column-1) == PLAYER_1 && next_mov.getWinner() != NO_PLAYER){
            //     this->state[line-1][column-1].push(Piece(currentPlayer, pieceSize));
            //     this->p2Pieces[pieceSize-1]--;
            // }
            else{
                return INVALID_ENTRY;
            }
        }else{
            return INVALID_ENTRY;
        }
        return OK;
    }else{
        return INVALID_TARGET;
    }
}

StatusEnum Board::movePiece(int sourceLine, int sourceColumn, int targetLine, int targetColumn){
    Piece old_piece;
    if(sourceLine>3 || sourceLine < 1 || sourceColumn>3 || sourceColumn < 1 || (sourceLine == targetLine && sourceColumn == targetColumn)){
        return INVALID_ENTRY;
    }else if(getPieceSize(sourceLine-1, sourceColumn-1) <= getPieceSize(targetLine-1,targetColumn-1)){
        return INVALID_ENTRY;
    }else if(this->state[sourceLine-1][sourceColumn-1].canPop() && getWinner() != nextPlayer(getPlaceHolder(sourceLine-1,sourceColumn-1))){
        old_piece = this->state[sourceLine-1][sourceColumn-1].peek();
        this->state[sourceLine-1][sourceColumn-1].pop();
        if(this->state[targetLine-1][targetColumn-1].canPush(old_piece)){
            this->state[targetLine-1][targetColumn-1].push(old_piece);
            return OK;
        }else{
            return INVALID_TARGET;
        }
    }else if(this->state[sourceLine-1][sourceColumn-1].canPop() && !(getWinner() != nextPlayer(getPlaceHolder(sourceLine-1,sourceColumn-1)))){
        old_piece = this->state[sourceLine-1][sourceColumn-1].peek();
        if(this->state[targetLine-1][targetColumn-1].canPush(old_piece) && getPlaceHolder(targetLine-1,targetColumn-1) == nextPlayer(getPlaceHolder(sourceLine-1,sourceColumn-1))){
            this->state[sourceLine-1][sourceColumn-1].pop();
            this->state[targetLine-1][targetColumn-1].push(old_piece);
            return OK;
        }else{
            return INVALID_TARGET;
        }
    }else{
        return INVALID_SOURCE;
    }
}

Player Board::getWinner(){
    int win_p1 = 0;
    int win_p2 = 0;
    // Verifica linhas e colunas
    for (int i = 0; i < 3; i++) {
        int line_p1 = 0, line_p2 = 0;
        int col_p1 = 0, col_p2 = 0;
        for (int j = 0; j < 3; j++) {
            // Linha i
            if (getPlaceHolder(i,j) == PLAYER_1) line_p1++;
            else if (getPlaceHolder(i,j) == PLAYER_2) line_p2++;

            // Coluna i
            if (getPlaceHolder(j,i) == PLAYER_1) col_p1++;
            else if (getPlaceHolder(j,i) == PLAYER_2) col_p2++;

        }
        if (line_p1 == 3) win_p1++;
        if (line_p2 == 3) win_p2++;
        if (col_p1 == 3) win_p1++;
        if (col_p2 == 3) win_p2++;
    }
    // Verifica diagonais
    int diag1_p1 = 0, diag1_p2 = 0;
    int diag2_p1 = 0, diag2_p2 = 0;

    for (int i = 0; i < 3; i++) {
        // Diagonal principal 
        if (getPlaceHolder(i,i) == PLAYER_1) diag1_p1++;
        else if (getPlaceHolder(i,i) == PLAYER_2) diag1_p2++;

        // Diagonal secundária /
        if (getPlaceHolder(i, 2-i) == PLAYER_1) diag2_p1++;
        else if (getPlaceHolder(i, 2-i)  == PLAYER_2) diag2_p2++;
    }
    if (diag1_p1 == 3) win_p1++;
    if (diag1_p2 == 3) win_p2++;
    if (diag2_p1 == 3) win_p1++;
    if (diag2_p2 == 3) win_p2++;
    // Retorna vencedor
    if (win_p1 > win_p2) return PLAYER_1;
    else if (win_p2 > win_p1) return PLAYER_2;
    else return NO_PLAYER;
}

std::ostream& Board::printHouses(std::ostream& stream, Player player){
    int* houses = getHousesOfPlayer(player);
    int sizes[3] = {SMALL, MEDIUM, LARGE};
    char iconsP1[3] = {SMALLP1, MEDIUMP1, LARGEP1};
    char iconsP2[3] = {SMALLP2, MEDIUMP2, LARGEP2};

    for(int s = 0; s < 3; s++){
        int n = houses[s]; // quantidade de peças desse tamanho
        for(int i = 0; i < 2; i++){ // sempre duas posições
            if(i < n){
                stream << ((player == PLAYER_1) ? iconsP1[s] : iconsP2[s]) << " ";
            } else {
                stream << "  "; // espaço quando não há peça
            }
        }
    }
    stream << "\n";
    return stream;
}



std::ostream& operator<<(std::ostream& stream, Board& board){
    char values[3][3];
    char icons[2][4] = {{' ',SMALLP1,MEDIUMP1,LARGEP1},{' ',SMALLP2,MEDIUMP2,LARGEP2}};
    for(int i=0; i<3;i++){
        for (int j = 0; j < 3; j++){
            if(board.getPlaceHolder(i,j) == PLAYER_1){
                values[i][j] = icons[0][board.getPieceSize(i,j)];
            }else if(board.getPlaceHolder(i,j) == PLAYER_2){
                values[i][j] = icons[1][board.getPieceSize(i,j)];
            }else if(board.getPlaceHolder(i,j) == NO_PLAYER){
                values[i][j] = ' ';
            }else{
                values[i][j] = ' ';
            }
        }
        
    }
    stream << "--------------------------------------------------";
    // Player winner = board.getWinner();
    // if(winner == NO_PLAYER){
    //     stream << "\nJoueur " << board << ", à vous de jouer :\n";
    // }
    stream << "\nÀ toi, joueur suivant !:\n";
    stream << "\n+---+---+---+\n";
    stream << "| " << values[0][0] << " | " << values[0][1] << " | " << values[0][2] << " |\t";
    stream << "Contenu des maisons";
    stream << "\n+---+---+---+\n";
    stream << "| " << values[1][0] << " | " << values[1][1] << " | " << values[1][2] << " |\t";
    board.printHouses(stream, PLAYER_1);
    stream << "+---+---+---+\n";
    stream << "| " << values[2][0] << " | " << values[2][1] << " | " << values[2][2] << " |\t";
    board.printHouses(stream, PLAYER_2);
    stream << "+---+---+---+\n";
    return stream;
}