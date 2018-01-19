#ifndef __GAME_H__
#define __GAME_H__
#include <iostream>
#include <vector>
#include "cell.h"
#include "piece.h"
#include "player.h"
#include "computer.h"
#include <vector>

const int gridSize = 8;

class Cell;
class Computer;

class GameNotification {
    public:
        virtual void notify(int r, int c, char rank) = 0;
};

class Game {
    char turn;
    GameNotification *notification;  // object registered for notifications
    void clearGame();

    public:
    Game();
    ~Game();
    Cell **theGrid;
    Cell *bk; // black king
    Cell *wk; // white king
    std::vector <Cell *> bqueens; //black queen
    std::vector <Cell *> wqueens; //white queen, etc
    std::vector <Cell *> bbishops;
    std::vector <Cell *> wbishops;
    std::vector <Cell *> bknights;
    std::vector <Cell *> wknights;
    std::vector <Cell *> brooks;
    std::vector <Cell *> wrooks;
    std::vector <Cell *> bpawns;
    std::vector <Cell *> wpawns;

    bool isCheck;
    std::vector <Cell *> kingsave;
    Cell * save;
    int pass;
    void setEnPassant();
    bool basicCheck(int old_r, int old_c, int new_r, int new_c);
    void notify(int r, int c);
    void notify(int old_r, int old_c, int new_r, int new_c,char promo='0');
    bool rookBlock(int r, int c, int dr, int dc);
    bool bishopBlock(int r,int c,int dr, int dc);
    bool queenBlock(int r, int c, int dr, int dc);
    bool blockCheck(Cell *starting, int new_r, int new_c);
    bool canBeEaten(char rank, int r, int c, bool on = true);
    bool hasPawn();
    void init(GameNotification *gameNotification);
    void init(int r, int c, char piece);
    void takeOff(int r, int c);
    void setTurn(char turn);
    char getTurn();
    void switchTurn();

    bool updatePieces(std::vector <Cell *> pieces);
    bool updatePawns(std::vector <Cell *> pawns);

    void enpassReset();
    bool checkmateCheck();
    bool stalemateCheck();
    bool gridCheck(int r, int c);
    bool cantMoveKing(Cell *king);
    bool allVectorsEmpty();
    void computerMove(Computer *comp);
    void testKingsave(bool on=true);
};


#endif
