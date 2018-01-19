#include "player.h"
#include <iostream>

using namespace std;

/***********initialization of White and Black***************
 * purpose: set memory position for future intances of Player
 ***********************************/
Player *Player::white = NULL;
Player *Player::black = NULL;

// player ctor
Player::Player(char colour, bool human, bool computer){
    this->colour = colour;
    this->human = human;
    this->computer = computer;
}
// returns the colour of the player
char Player::getColour(){
  return colour;
}

// player dtor
Player::~Player(){
    if (!white){
		delete white;
	}
	if (!black){
		delete black;
	}
	white = NULL;
	black = NULL;

}
