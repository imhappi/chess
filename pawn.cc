#include "pawn.h"
#include <iostream>

/***Pawn***
 * Pawn constructor
 *************/
Pawn::Pawn(char rank){
    this->rank = rank;
    this->first = true;
    enpassant = false;

    if (rank == 'P'){ // if it's a white piece
        this->colour = 'w';
    } else if (rank == 'p'){ // if it's a black piece
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
bool Pawn::move(int old_r, int old_c, int new_r, int new_c){
    // if the new position is 1 in front of the old one
    if (colour == 'b'){
        // if it's black, it needs to move forward from the top
        if ((new_r == old_r + 1) && (new_c == old_c)){
            return true;
            // else if it's the first move, pawns can move 2 squares
        } else if (first && ((new_r == old_r + 2) && (new_c == old_c))){
            return true;

        }
        // if it's white, it needs to move forward from the bottom
    } else if (colour == 'w'){
        if ((new_r == old_r - 1) && (new_c == old_c)){
            return true;
            // else if it's the first move, pawns can move 2 squares
        } else if (first && ((new_r == old_r - 2) && (new_c == old_c))){
            return true;
        }
    }

    return false;
}

/****eat****
 * returns true if the piece at position
 * r/c can eat the piece at position new_r/new_c.
 * returns false otherwise
 ************/
bool Pawn::eat(int old_r, int old_c, int new_r, int new_c){
    // if new position 1 diagonal forward to the old one
    if (colour == 'w'){
        // if it's white, it needs to move from the top
        if ((new_r == old_r - 1) && ((new_c == old_c - 1) || (new_c == old_c + 1))){
            return true;
        }
        // if it's black, it needs to move from the bottom
    } else if (colour == 'b'){
        if ((new_r == old_r + 1) && ((new_c == old_c - 1) || (new_c == old_c + 1))){
            return true;
        }
    }

    return false;
}

