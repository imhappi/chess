#ifndef __HUMAN_H_
#define __HUMAN_H_

#include "player.h"
#include <string>

class Human : public Player{
	Human(char c);
public:
	//~Human();
  static Player *createWhite(char c);
  static Player *createBlack(char c);

  std::string getMove();
};

#endif
