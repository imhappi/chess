#include "human.h"
#include <string>
#include <iostream>
#include <sstream>

Player *Human::createWhite(char c) {
  if (!white) { // if instance is not set, set it
    white = new Human(c);
  } //if
  return white;
}

Player *Human::createBlack(char c) {
  if (!black) { // if instance is not set, set it
    black = new Human(c);
  } //if
  return black;
}

// human ctor
Human::Human(char c):Player(c,true,false){}

/***getMove****
 * returns the string of the input
 * from the player
***************/
std::string Human::getMove(){
    std::string move = "";
  std::string input, cmd;
  getline(std::cin,input);
  std::stringstream ss(input);
  while(ss >> cmd){
    move += cmd;
  }
  return move;
}
