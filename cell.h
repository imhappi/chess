#ifndef __CELL_H__
#define __CELL_H__
#include <iostream>
#include "game.h"
#include "piece.h"
#include <vector>

class Game;

class Cell {
    Piece* state;              //current state of the cell
    int r, c;                   //co-ordinates of the cell
    Game * game;                //pointer to game
    std::vector<Cell *> possibleEats;
    void notifyGame();	     // notify the game and registered notification object of the change
    bool withinBoard(int r,int c);

  public:
    Cell();
    ~Cell();
    //get functions
    int getR();
    int getC();
    char getRank();
    char getColour();
    bool getEnpass();
    bool getFirst();
    Piece * getState();
    void eatPiece(int new_r, int new_c);
    void movePiece(int new_r, int new_c);
    void enpassPiece(int new_r,int new_c);
    void eatPromotePiece(int new_r, int new_c, char promo);
    void promotePiece(int new_r,int new_c,char promo);
    void castlingPiece(int new_r,int new_c);
    void clearEats();
    void tempSetState(Piece *piece);          // temporarily setter for state
    void setStateplus(Piece *piece);
    void setStatemin();
    void setCoords(const int r, const int c);
    void selfpush(char rank);
    void setGame(Game * game);
    void addRook();
    void addBishop();
    void addToEats();
    std::vector<Cell *> getEats();
    void addAdd(Cell* c);
    void eraseErase(int index);
    bool stillInCheck(int new_r, int new_c);
};
#endif
