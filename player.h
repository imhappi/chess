#ifndef __PLAYER_H_
#define __PLAYER_H_

#include <cstdlib>
#include <string>

class Player{
protected:
	static Player * white;
	static Player * black;
    char colour;
    Player(char colour, bool human, bool computer);
public:
    bool computer;
    bool human;
    char getColour();

    virtual std::string getMove() = 0;
    virtual ~Player();
};

#endif
