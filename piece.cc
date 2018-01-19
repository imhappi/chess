#include "piece.h"

// returns the piece's colour
char Piece::getColour(){
    return colour;
}
// returns the piece's rank
char Piece::getRank(){
    return rank;
}
// returns whether or not it has moved yet;
// true if it hasn't, false if it is
bool Piece::getFirst(){
	return first;
}
// sets whether or not it has moved yet
void Piece::setFirst(bool first){
    this->first = first;
}
// sets whether it has access to enpassant
void Piece::setEnpass(bool enpass){
	enpassant = enpass;
}
// returns the enpassant value of the cell
bool Piece::getEnpass(){
	return enpassant;
}
// piece dtor
Piece::~Piece() {}
