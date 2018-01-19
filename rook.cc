#include "rook.h"

/***Rook****
 * Rook constructor
 ************/
Rook::Rook(char rank){
    this->rank = rank;
    this->first = true;

    if (rank == 'R'){ // if it's a white piece
        this->colour = 'w';
    } else if (rank == 'r'){ // if it's a black piece
        this->colour = 'b';
    } else { // for debugging purposes
        this->colour = 'X';
    }
}

/****move****
 * returns true if the move from
 * r/c to new_r/new_c is valid. returns
 * false otherwise
 *************/
bool Rook::move(int old_r, int old_c, int new_r, int new_c){
    // if the move is purely horizontal
    if (old_r == new_r && old_c != new_c){
        return true;
    } else if (old_c == new_c && old_r != new_r){ // if the move is purely vertical
        return true;
    }
    
    return false;
}

/****eat****
 * returns true if the piece at position
 * r/c can eat the piece at position new_r/new_c.
 * returns false otherwise
 ************/
bool Rook::eat(int old_r, int old_c, int new_r, int new_c){
    return this->move(old_r,old_c,new_r,new_c);
}
