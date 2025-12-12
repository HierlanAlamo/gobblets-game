#include <iostream>
#include <string>
#include "board.h"
#include "piece.h"
#include "config.h"
#include "cell.h"

using namespace std;

Cell::Cell(){
    this->index = -1;
    for(int i=0; i< NB_SIZE;i++){
        this->stack[i] = Piece();
    }
}

Cell::Cell(const Cell& other){
    this->index = other.index;
    for(int i=0; i<=index; i++){
        this->stack[i] = other.stack[i];
    }
}

bool Cell::canPush(const Piece& piece){
    if(index < 0) return true;
    return piece.getSize() > stack[index].getSize();
}

void Cell::push(const Piece& piece){
    if(this->canPush(piece)){
        index++;
        stack[index] = piece;
    }
}

bool Cell::canPop(){
    return (index>=0 && index < 3);
}


Piece& Cell::pop() {
    if (canPop()) {
        return stack[index--];
    }else{
        throw std::runtime_error("Cannot pop from empty cell");
    }
    
}

Piece& Cell::peek() {
    return stack[index];
}

Cell& Cell::operator=(const Cell& other){
    if(this != &other){
        this->index = other.index;
        for(int i=0; i<=index; i++){
            this->stack[i] = other.stack[i];
        }
    }
    return *this; 
}