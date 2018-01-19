#ifndef __COMPUTER_H_
#define __COMPUTER_H_

#include "player.h"
#include <string>
#include <vector>
#include "game.h"
#include "cell.h"


class Game;
class Cell;

class Computer : public Player{
  //int level;
  Game *game;
  Cell *prevMove;
  Cell *currentBestMove;
  Computer(char c,int level, Game *game);
public:

  static Player *createWhiteAi(char c, int i, Game *game);
  static Player *createBlackAi(char c, int i, Game *game);
  int level;

  std::string getMove();
  Cell * getP(std::vector<Cell *> piece);
  Cell *getPrev();
  int getPiece(int n);
  void pawnMove();
  void castlingMove();
  void level1(bool level4 = false);
  void level2();
  void level3();
  void level4();
  bool canEat(Cell *piece, bool lvl4);
  bool canCheck(Cell *piece, bool lvl4);
  void level2White(bool level4 = false);
  void level2Black(bool level4 = false);
  bool canDie(Cell *piece, bool lvl4);
  void level3White(bool level4 = false);
  void level3Black(bool level4 = false);
  void level4White();
  void level4Black();
  bool inDanger(Cell *piece, int new_r, int new_c);
};

#endif
