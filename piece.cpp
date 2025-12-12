#include <iostream>
#include <string>
#include "board.h"
#include "playerEnum.h"
#include "piece.h"

using namespace std;

Player Piece::getOwner() const{
    return this->owner;
}

Size Piece::getSize() const{
    return this->size;
}

Piece::Piece(){
    this->owner = NO_PLAYER;
    this->size = NONE;
}

Piece::Piece(Player owner, Size size){
    this->owner = owner;
    this->size = size;
}
Piece::Piece(const Piece& other){
    this->owner = other.owner;
    this->size = other.size;
}

Piece& Piece::operator=(const Piece& other) {
    if(this != &other){
        this->owner = other.owner;
        this->size = other.size;
    }
    return *this; 
}