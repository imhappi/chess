#include "queen.h"

/***Queen****
 * Queen constructor
 ************/
Queen::Queen(char rank){
    this->rank = rank;

    if (rank == 'Q'){ // if it's a white piece
        this->colour = 'w';
    } else if (rank == 'q'){ // if it's a black piece
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
bool Queen::move(int old_r, int old_c, int new_r, int new_c){
    // if the move is only sideways
    if (old_r == new_r && old_c != new_c){
        return true;
    } else if (old_c == new_c && old_r != new_r){ // if the move is only up/down
        return true;
    } else if ((new_c + new_r == old_c + old_r) && (new_c != old_c)){ // if move is diagonal
        return true;
    } else if ((old_r - new_r == old_c - new_c) && ((new_c != old_c) && (new_r != old_r))){ // if move is diagonal
        return true;
    }
    
    return false;
}

/****eat****
 * returns true if the piece at position
 * r/c can eat the piece at position new_r/new_c.
 * returns false otherwise
 ************/
bool Queen::eat(int old_r, int old_c, int new_r, int new_c){
    return this->move(old_r,old_c,new_r,new_c);
}
