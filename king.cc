#include "king.h"

/***King****
 * King constructor
 ************/
King::King(char rank){
    this->rank = rank;
    this->first = true;

    if (rank == 'K'){ // if it's a white piece
        this->colour = 'w';
    } else if (rank == 'k'){ // if it's a black piece
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
bool King::move(int old_r, int old_c, int new_r, int new_c){ //CASTLING ?
    // the new position is a diagonal
    if (((new_c == old_c - 1) || (new_c == old_c + 1)) && ((new_r == old_r - 1) || (new_r == old_r + 1))){
        return true;
    } else if ((new_c == old_c) && ((new_r == old_r - 1) || (new_r == old_r + 1))){ // can move sideways
        return true;
    } else if (((new_c == old_c - 1) || (new_c == old_c + 1)) && (new_r == old_r)){ // can move up/down
        return true;
    } else if (((new_c == old_c - 2) || (new_c == old_c + 2)) && (new_r == old_r)){
        return true;
    }
    return false;
}

/****eat****
 * returns true if the piece at position
 * r/c can eat the piece at position new_r/new_c.
 * returns false otherwise
 ************/
bool King::eat(int old_r, int old_c, int new_r, int new_c){
  if (((new_c == old_c - 1) || (new_c == old_c + 1)) && ((new_r == old_r - 1) || (new_r == old_r + 1))){
      return true;
  } else if ((new_c == old_c) && ((new_r == old_r - 1) || (new_r == old_r + 1))){ // can move sideways
      return true;
  } else if (((new_c == old_c - 1) || (new_c == old_c + 1)) && (new_r == old_r)){ // can move up/down
      return true;
  }
  return false;
}
