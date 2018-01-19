#include "bishop.h"

/***Bishop****
 * Bishop constructor
 ************/
Bishop::Bishop(char rank){
    this->rank = rank;

    if (rank == 'B'){ // if it's a white piece
        this->colour = 'w';
    } else if (rank == 'b'){ // if it's a black piece
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
bool Bishop::move(int old_r, int old_c, int new_r, int new_c){
    // if the move is diagonal from top left to bottom right
    if ((new_c + new_r == old_c + old_r) && (new_c != old_c)){
        return true;
    } else if ((old_r - new_r == old_c - new_c) && ((new_c != old_c) && (new_r != old_r))){ // if diagonal the other way
        return true;
    }
    
    return false;
}

/****eat****
 * returns true if the piece at position
 * r/c can eat the piece at position new_r/new_c.
 * returns false otherwise
 ************/
bool Bishop::eat(int old_r, int old_c, int new_r, int new_c){
    return this->move(old_r,old_c,new_r,new_c);
}
