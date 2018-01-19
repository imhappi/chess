#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include <iostream>
#include "game.h"
#include "view.h"
#include "human.h"


class Computer;

class Controller : public GameNotification {

    View *td; // The text display.
    View *gd; // The graphic display.
    bool setup;
    Game *game;

    Player *white;
    Player *black;
    void init(std::istream & input, Game & g, bool saved=false);

  public:
    Controller();
    ~Controller();
    void printScore();
    void savemode(std::string saved);
    void gamemode();
    void setupmode();
    void movemode();
    char play(bool save=0,std::string saved="");
    void notify(int r, int c, char rank);
    char finish(char losingPlayer);
    void checkFinish();
};

#endif
