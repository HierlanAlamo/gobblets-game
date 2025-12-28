#include <iostream>
#include <string>
#include "board.h"
#include "bot.h"
#include <cstdlib>   // rand(), srand()
#include <ctime> // to create random seeds
using namespace std;


Size Bot::randSize(){
    return static_cast<Size>(rand() % NB_SIZE + 1);
}

int Bot::randMove(){
    return rand() % 2 + 1;
}

int Bot::randLC(){ // creates a random line and column
    return rand() % DIMENSIONS + 1;  // returns 1, 2 ou 3
}

void Bot::selectMode(Mode gameMode){
    this->gameMode = gameMode;
}

Bot::Bot(){
    srand(time(nullptr)); // creates a new seed each time
    this->gameMode = NORMAL;
}

Bot::Bot(Mode gameMode){
    srand(time(nullptr)); // creates a new seed each time
    this->gameMode = gameMode;
}

StatusEnum Bot::botTurn(Board& board){
    StatusEnum result;
    
    if(this->gameMode == EASY){
        int nextMove;
        do{
            nextMove = randMove();
            if(nextMove == 1){
                if(board.getNbPiecesInHouse(PLAYER_2, randSize()) > 0){
                    result = board.placePiece(PLAYER_2, randSize(), randLC(), randLC());
                }else{
                    result = INVALID_ENTRY;
                }
            }else if(nextMove == 2){
                int l1 = randLC(), l2 = randLC(), c1 = randLC(), c2 = randLC();
                if(board.getPlaceHolder(l1-1, c1-1) == PLAYER_1){
                    result = INVALID_ENTRY;
                }else{
                    result = board.movePiece(l1, c1, l2, c2);
                }
            }
        }while(result != OK);
        return result;
        
    } else if(this->gameMode == NORMAL){
        // NORMAL mode: basic strategies
        Size sizes[3] = {SMALL, MEDIUM, LARGE};
        
        // 1. Try to win by placing a piece
        for(int s = 0; s < 3; s++){
            if(board.getNbPiecesInHouse(PLAYER_2, sizes[s]) == 0) continue;
            
            for(int l = 1; l <= 3; l++){
                for(int c = 1; c <= 3; c++){
                    // Test the move directly without creating a copy
                    // Save original state to restore if needed
                    Board originalBoard = board;
                    result = board.placePiece(PLAYER_2, sizes[s], l, c);
                    if(result == OK && board.getWinner() == PLAYER_2){
                        return OK; // Success!
                    }else{
                        // Restore original board if move didn't win
                        board = originalBoard;
                    }
                }
            }
        }
        
        // 2. Try to win by moving a piece
        for(int srcL = 1; srcL <= 3; srcL++){
            for(int srcC = 1; srcC <= 3; srcC++){
                if(board.getPlaceHolder(srcL-1, srcC-1) != PLAYER_2) continue;
                
                for(int dstL = 1; dstL <= 3; dstL++){
                    for(int dstC = 1; dstC <= 3; dstC++){
                        if(srcL == dstL && srcC == dstC) continue;
                        
                        Board originalBoard = board;
                        result = board.movePiece(srcL, srcC, dstL, dstC);
                        if(result == OK && board.getWinner() == PLAYER_2){
                            return OK; // Success!
                        }else{
                            board = originalBoard;
                        }
                    }
                }
            }
        }
        
        // 3. Block PLAYER_1 if they can win
        // Check if PLAYER_1 can win by placing a piece
        for(int s = 0; s < 3; s++){
            for(int l = 1; l <= 3; l++){
                for(int c = 1; c <= 3; c++){
                    Board originalBoard = board;
                    result = board.placePiece(PLAYER_1, sizes[s], l, c);
                    if(result == OK && board.getWinner() == PLAYER_1){
                        // Restore board and try to block
                        board = originalBoard;
                        
                        // Try to block with larger piece
                        for(int s2 = 2; s2 >= 0; s2--){
                            if(board.getNbPiecesInHouse(PLAYER_2, sizes[s2]) > 0){
                                if(sizes[s2] > board.getPieceSize(l-1, c-1)){
                                    result = board.placePiece(PLAYER_2, sizes[s2], l, c);
                                    if(result == OK) return OK;
                                }
                            }
                        }
                    }else{
                        board = originalBoard;
                    }
                }
            }
        }
        
        // Check if PLAYER_1 can win by moving a piece
        for(int srcL = 1; srcL <= 3; srcL++){
            for(int srcC = 1; srcC <= 3; srcC++){
                if(board.getPlaceHolder(srcL-1, srcC-1) != PLAYER_1) continue;
                
                for(int dstL = 1; dstL <= 3; dstL++){
                    for(int dstC = 1; dstC <= 3; dstC++){
                        if(srcL == dstL && srcC == dstC) continue;
                        
                        Board originalBoard = board;
                        result = board.movePiece(srcL, srcC, dstL, dstC);
                        if(result == OK && board.getWinner() == PLAYER_1){
                            // Restore board and try to block destination
                            board = originalBoard;
                            
                            for(int s2 = 2; s2 >= 0; s2--){
                                if(board.getNbPiecesInHouse(PLAYER_2, sizes[s2]) > 0){
                                    if(sizes[s2] > board.getPieceSize(dstL-1, dstC-1)){
                                        result = board.placePiece(PLAYER_2, sizes[s2], dstL, dstC);
                                        if(result == OK) return OK;
                                    }
                                }
                            }
                        }else{
                            board = originalBoard;
                        }
                    }
                }
            }
        }
        
        // 4. Offensive strategies - prioritize center
        // Try to place in center (2,2)
        for(int s = 2; s >= 0; s--){
            if(board.getNbPiecesInHouse(PLAYER_2, sizes[s]) > 0){
                result = board.placePiece(PLAYER_2, sizes[s], 2, 2);
                if(result == OK) return OK;
            }
        }
        
        // Try to move to center
        for(int srcL = 1; srcL <= 3; srcL++){
            for(int srcC = 1; srcC <= 3; srcC++){
                if(board.getPlaceHolder(srcL-1, srcC-1) != PLAYER_2) continue;
                
                result = board.movePiece(srcL, srcC, 2, 2);
                if(result == OK) return OK;
            }
        }
        
        // Try to place in corners
        int corners[4][2] = {{1,1}, {1,3}, {3,1}, {3,3}};
        for(int i = 0; i < 4; i++){
            for(int s = 2; s >= 0; s--){
                if(board.getNbPiecesInHouse(PLAYER_2, sizes[s]) > 0){
                    result = board.placePiece(PLAYER_2, sizes[s], corners[i][0], corners[i][1]);
                    if(result == OK) return OK;
                }
            }
        }
        
        // 5. Intelligent random move (don't waste large pieces)
        int attempts = 0;
        while(attempts < 20){
            int moveType = randMove();
            
            if(moveType == 1){ // Place piece
                // Prioritize medium and small pieces first
                int sizeOrder[3] = {1, 0, 2}; // MEDIUM, SMALL, LARGE
                for(int i = 0; i < 3; i++){
                    Size s = sizes[sizeOrder[i]];
                    if(board.getNbPiecesInHouse(PLAYER_2, s) > 0){
                        int l = randLC();
                        int c = randLC();
                        result = board.placePiece(PLAYER_2, s, l, c);
                        if(result == OK) return OK;
                    }
                }
            }else{ // Move piece
                int srcL = randLC();
                int srcC = randLC();
                int dstL = randLC();
                int dstC = randLC();
                
                if(board.getPlaceHolder(srcL-1, srcC-1) == PLAYER_2){
                    result = board.movePiece(srcL, srcC, dstL, dstC);
                    if(result == OK) return OK;
                }
            }
            attempts++;
        }
        
        // 6. Last resort: any valid move
        for(int l = 1; l <= 3; l++){
            for(int c = 1; c <= 3; c++){
                for(int s = 0; s < 3; s++){
                    if(board.getNbPiecesInHouse(PLAYER_2, sizes[s]) > 0){
                        result = board.placePiece(PLAYER_2, sizes[s], l, c);
                        if(result == OK) return OK;
                    }
                }
            }
        }
        
        for(int srcL = 1; srcL <= 3; srcL++){
            for(int srcC = 1; srcC <= 3; srcC++){
                if(board.getPlaceHolder(srcL-1, srcC-1) != PLAYER_2) continue;
                
                for(int dstL = 1; dstL <= 3; dstL++){
                    for(int dstC = 1; dstC <= 3; dstC++){
                        if(srcL == dstL && srcC == dstC) continue;
                        result = board.movePiece(srcL, srcC, dstL, dstC);
                        if(result == OK) return OK;
                    }
                }
            }
        }
        
        return INVALID_ENTRY;
        
    }else if(this->gameMode == HARD){
        // HARD mode: advanced strategies with position evaluation
        Size sizes[3] = {SMALL, MEDIUM, LARGE};
        
        // 1. Try to win immediately (same as NORMAL)
        for(int s = 0; s < 3; s++){
            if(board.getNbPiecesInHouse(PLAYER_2, sizes[s]) == 0) continue;
            
            for(int l = 1; l <= 3; l++){
                for(int c = 1; c <= 3; c++){
                    Board originalBoard = board;
                    result = board.placePiece(PLAYER_2, sizes[s], l, c);
                    if(result == OK && board.getWinner() == PLAYER_2){
                        return OK;
                    }else{
                        board = originalBoard;
                    }
                }
            }
        }
        
        for(int srcL = 1; srcL <= 3; srcL++){
            for(int srcC = 1; srcC <= 3; srcC++){
                if(board.getPlaceHolder(srcL-1, srcC-1) != PLAYER_2) continue;
                for(int dstL = 1; dstL <= 3; dstL++){
                    for(int dstC = 1; dstC <= 3; dstC++){
                        if(srcL == dstL && srcC == dstC) continue;
                        Board originalBoard = board;
                        result = board.movePiece(srcL, srcC, dstL, dstC);
                        if(result == OK && board.getWinner() == PLAYER_2){
                            return OK;
                        }else{
                            board = originalBoard;
                        }
                    }
                }
            }
        }
        
        // 2. Block PLAYER_1 (same as NORMAL)
        for(int s = 0; s < 3; s++){
            for(int l = 1; l <= 3; l++){
                for(int c = 1; c <= 3; c++){
                    Board originalBoard = board;
                    result = board.placePiece(PLAYER_1, sizes[s], l, c);
                    if(result == OK && board.getWinner() == PLAYER_1){
                        board = originalBoard;
                        for(int s2 = 2; s2 >= 0; s2--){
                            if(board.getNbPiecesInHouse(PLAYER_2, sizes[s2]) > 0){
                                if(sizes[s2] > board.getPieceSize(l-1, c-1)){
                                    result = board.placePiece(PLAYER_2, sizes[s2], l, c);
                                    if(result == OK) return OK;
                                }
                            }
                        }
                    }else{
                        board = originalBoard;
                    }
                }
            }
        }
        
        for(int srcL = 1; srcL <= 3; srcL++){
            for(int srcC = 1; srcC <= 3; srcC++){
                if(board.getPlaceHolder(srcL-1, srcC-1) != PLAYER_1) continue;
                
                for(int dstL = 1; dstL <= 3; dstL++){
                    for(int dstC = 1; dstC <= 3; dstC++){
                        if(srcL == dstL && srcC == dstC) continue;
                        
                        Board originalBoard = board;
                        result = board.movePiece(srcL, srcC, dstL, dstC);
                        if(result == OK && board.getWinner() == PLAYER_1){
                            board = originalBoard;
                            for(int s2 = 2; s2 >= 0; s2--){
                                if(board.getNbPiecesInHouse(PLAYER_2, sizes[s2]) > 0){
                                    if(sizes[s2] > board.getPieceSize(dstL-1, dstC-1)){
                                        result = board.placePiece(PLAYER_2, sizes[s2], dstL, dstC);
                                        if(result == OK) return OK;
                                    }
                                }
                            }
                        }else{
                            board = originalBoard;
                        }
                    }
                }
            }
        }
        
        // 3. Evaluate best positions with point system
        int bestScore = -1000;
        int bestMoveType = 0; // 1=place, 2=move
        int bestData[5] = {0}; // move data
        
        // Evaluate piece placements
        for(int s = 0; s < 3; s++){
            if(board.getNbPiecesInHouse(PLAYER_2, sizes[s]) == 0) continue;
            for(int l = 1; l <= 3; l++){
                for(int c = 1; c <= 3; c++){
                    Board originalBoard = board;
                    result = board.placePiece(PLAYER_2, sizes[s], l, c);
                    if(result != OK){
                        board = originalBoard;
                        continue;
                    }
                    
                    int score = 0;
                    
                    // Bonus for center
                    if(l == 2 && c == 2) score += 20;
                    
                    // Bonus for corners
                    if((l == 1 && c == 1) || (l == 1 && c == 3) || 
                       (l == 3 && c == 1) || (l == 3 && c == 3)) score += 10;
                    
                    // Bonus for creating threats
                    Player winner = board.getWinner();
                    if(winner == PLAYER_2){
                        score += 1000;
                    }else{
                        // Check almost complete lines
                        for(int i = 0; i < 3; i++){
                            int p2_in_row = 0, p1_in_row = 0;
                            int p2_in_col = 0, p1_in_col = 0;
                            
                            for(int j = 0; j < 3; j++){
                                if(board.getPlaceHolder(i, j) == PLAYER_2) p2_in_row++;
                                else if(board.getPlaceHolder(i, j) == PLAYER_1) p1_in_row++;
                                
                                if(board.getPlaceHolder(j, i) == PLAYER_2) p2_in_col++;
                                else if(board.getPlaceHolder(j, i) == PLAYER_1) p1_in_col++;
                            }
                            
                            if(p2_in_row == 2 && p1_in_row == 0) score += 30;
                            if(p2_in_col == 2 && p1_in_col == 0) score += 30;
                        }
                    }
                    
                    // Penalty for using large piece unnecessarily
                    if(sizes[s] == LARGE && originalBoard.getPieceSize(l-1, c-1) == NONE){
                        score -= 5;
                    }
                    
                    // Restore board and save score
                    board = originalBoard;
                    
                    if(score > bestScore){
                        bestScore = score;
                        bestMoveType = 1;
                        bestData[0] = s;
                        bestData[1] = l;
                        bestData[2] = c;
                    }
                }
            }
        }
        
        // Evaluate piece movements
        for(int srcL = 1; srcL <= 3; srcL++){
            for(int srcC = 1; srcC <= 3; srcC++){
                if(board.getPlaceHolder(srcL-1, srcC-1) != PLAYER_2) continue;
                
                for(int dstL = 1; dstL <= 3; dstL++){
                    for(int dstC = 1; dstC <= 3; dstC++){
                        if(srcL == dstL && srcC == dstC) continue;
                        
                        Board originalBoard = board;
                        result = board.movePiece(srcL, srcC, dstL, dstC);
                        if(result != OK){
                            board = originalBoard;
                            continue;
                        }
                        
                        int score = 0;
                        
                        // Bonus for moving to center
                        if(dstL == 2 && dstC == 2) score += 25;
                        
                        // Bonus for creating threats
                        Player winner = board.getWinner();
                        if(winner == PLAYER_2){
                            score += 1000;
                        } else {
                            // Check almost complete lines
                            for(int i = 0; i < 3; i++){
                                int p2_in_row = 0, p1_in_row = 0;
                                int p2_in_col = 0, p1_in_col = 0;
                                
                                for(int j = 0; j < 3; j++){
                                    if(board.getPlaceHolder(i, j) == PLAYER_2) p2_in_row++;
                                    else if(board.getPlaceHolder(i, j) == PLAYER_1) p1_in_row++;
                                    
                                    if(board.getPlaceHolder(j, i) == PLAYER_2) p2_in_col++;
                                    else if(board.getPlaceHolder(j, i) == PLAYER_1) p1_in_col++;
                                }
                                
                                if(p2_in_row == 2 && p1_in_row == 0) score += 35;
                                if(p2_in_col == 2 && p1_in_col == 0) score += 35;
                            }
                        }
                        
                        // Bonus for capturing opponent's piece
                        if(originalBoard.getPlaceHolder(dstL-1, dstC-1) == PLAYER_1){
                            score += 15;
                        }
                        
                        // Restore board and save score
                        board = originalBoard;
                        
                        if(score > bestScore){
                            bestScore = score;
                            bestMoveType = 2;
                            bestData[0] = srcL;
                            bestData[1] = srcC;
                            bestData[2] = dstL;
                            bestData[3] = dstC;
                        }
                    }
                }
            }
        }
        
        // Execute best move found
        if(bestMoveType == 1){
            Size pieceSize = sizes[bestData[0]];
            int line = bestData[1];
            int column = bestData[2];
            result = board.placePiece(PLAYER_2, pieceSize, line, column);
            if(result == OK) return OK;
        } else if(bestMoveType == 2){
            int srcLine = bestData[0];
            int srcColumn = bestData[1];
            int dstLine = bestData[2];
            int dstColumn = bestData[3];
            result = board.movePiece(srcLine, srcColumn, dstLine, dstColumn);
            if(result == OK) return OK;
        }
        
        // 4. Fallback: NORMAL mode strategy
        // Try center
        for(int s = 2; s >= 0; s--){
            if(board.getNbPiecesInHouse(PLAYER_2, sizes[s]) > 0){
                result = board.placePiece(PLAYER_2, sizes[s], 2, 2);
                if(result == OK) return OK;
            }
        }
        
        // Try to move to center
        for(int srcL = 1; srcL <= 3; srcL++){
            for(int srcC = 1; srcC <= 3; srcC++){
                if(board.getPlaceHolder(srcL-1, srcC-1) != PLAYER_2) continue;
                
                result = board.movePiece(srcL, srcC, 2, 2);
                if(result == OK) return OK;
            }
        }
        
        // Last resort: random move
        int attempts = 0;
        while(attempts < 30){
            int moveType = randMove();
            
            if(moveType == 1){
                Size s = randSize();
                if(board.getNbPiecesInHouse(PLAYER_2, s) > 0) {
                    int l = randLC();
                    int c = randLC();
                    result = board.placePiece(PLAYER_2, s, l, c);
                    if(result == OK) return OK;
                }
            }else{
                int srcL = randLC();
                int srcC = randLC();
                int dstL = randLC();
                int dstC = randLC();
                
                if(board.getPlaceHolder(srcL-1, srcC-1) == PLAYER_2){
                    result = board.movePiece(srcL, srcC, dstL, dstC);
                    if(result == OK) return OK;
                }
            }
            attempts++;
        }
        
        return INVALID_ENTRY;
    }
    
    return INVALID_ENTRY;
}