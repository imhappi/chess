#include "knight.h"

/***Knight***
 * Knight constructor
 *************/
Knight::Knight(char rank){
    this->rank = rank;

    if (rank == 'N'){ // if it's a white piece
        this->colour = 'w';
    } else if (rank == 'n'){ // if it's a black piece
        this->colour = 'b';
    } else { // for debugging purposes
        this->colour = 'N';
    }
}

/****move****
 * returns true if the move from
 * r/c to new_r/new_c is valid. returns
 * false otherwise
 *************/
bool Knight::move(int old_r, int old_c, int new_r, int new_c){
    // if the move is an L in 4 different ways
    if ((old_r == new_r - 1) && (old_c == new_c - 2)){
        return true;
    } else if ((old_r == new_r - 2) && (old_c == new_c - 1)){
        return true;
    } else if ((old_r == new_r - 2) && (old_c == new_c + 1)){
        return true;
    } else if ((old_r == new_r - 1) && (old_c == new_c + 2)){
        return true;
    } else if ((old_r == new_r + 1) && (old_c == new_c + 2)){
        return true;
    } else if ((old_r == new_r + 2) && (old_c == new_c + 1)){
        return true;
    } else if ((old_r == new_r + 2) && (old_c == new_c - 1)){
        return true;
    } else if ((old_r == new_r + 1) && (old_c == new_c - 2)){
        return true;
    }
    return false;
}

/****eat****
 * returns true if the piece at position
 * r/c can eat the piece at position new_r/new_c.
 * returns false otherwise
 ************/
bool Knight::eat(int old_r, int old_c, int new_r, int new_c){
    // if the eaten piece is in the path of an L
    return this->move(old_r,old_c,new_r,new_c);
}
