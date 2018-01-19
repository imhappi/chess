#include <iostream>
#include <string>
#include <sstream>
#include "game.h"
#include "piece.h"
#include "king.h"
#include "knight.h"
#include "queen.h"
#include "pawn.h"
#include "rook.h"
#include "bishop.h"

using namespace std;

// game ctor
Game::Game()
:turn('w'),notification(NULL), theGrid(NULL),bk(NULL),wk(NULL),isCheck(false),save(NULL),pass(0){}

// game dtor
Game::~Game(){

    if (theGrid){
        for (int i=0; i < gridSize; i++){
            delete[] theGrid[i];
        }
    }
    delete[] theGrid;
}

/****clearGame***
 * clears game; frees the grid
 * and resets everything
 *****************/
void Game::clearGame(){
    if(theGrid != NULL){
        for (int i=0; i < gridSize; i++){
            delete[] theGrid[i];
        }
    }
    delete[] theGrid;
}

/*****notify****
 * notifies all the displays that of the chang
 ****************/
void Game::notify(int r, int c){
    notification->notify(7-r,c,theGrid[r][c].getRank());
}

/****cantMoveKing****
 * returns true if you can't move the king,
 * and false otherwise
 *********************/
bool Game::cantMoveKing(Cell *king){
    int r = king->getR();
    int c = king->getC();
    char colour = king->getColour();
    if (gridCheck(r-1,c-1) && theGrid[r-1][c-1].getColour() != colour &&
            !king->stillInCheck(r-1,c-1)) {
        return false;
    }
    if (gridCheck(r-1,c) && theGrid[r-1][c].getColour() != colour &&
            !king->stillInCheck(r-1,c)) {
        return false;
    }
    if (gridCheck(r-1,c+1) && theGrid[r-1][c+1].getColour() != colour &&
            !king->stillInCheck(r-1,c+1)) {
        return false;
    }
    if (gridCheck(r,c-1) && theGrid[r][c-1].getColour() != colour &&
            !king->stillInCheck(r,c-1)) {
        return false ;
    }
    if (gridCheck(r,c+1) && theGrid[r][c+1].getColour() != colour &&
            !king->stillInCheck(r,c+1)) {
        return false;
    }
    if (gridCheck(r+1,c-1) && theGrid[r+1][c-1].getColour() != colour &&
            !king->stillInCheck(r+1,c-1)) {
        return false;
    }
    if (gridCheck(r+1,c) && theGrid[r+1][c].getColour() != colour &&
            !king->stillInCheck(r+1,c)) {
        return false;
    }
    if (gridCheck(r+1,c+1) && theGrid[r+1][c+1].getColour() != colour &&
            !king->stillInCheck(r+1,c+1)) {
        return false;
    }
    return true;
}

/****checkmateCheck****
 * checks if a king is in check for the
 * current player
 *******************/
bool Game::checkmateCheck(){
    // since king will never be eaten, and game cannot start without kings,
    // can assume that it will never be null]
    if (turn == 'b' && bk){
        int r = bk->getR();
        int c = bk->getC();
        if (canBeEaten('b',r,c)){
            isCheck = true;
            if (cantMoveKing(bk)){
                for (vector<Cell *>::iterator i = kingsave.begin(); i != kingsave.end() ; i++) {
                    int saveR = (*i)->getR();
                    int saveC = (*i)->getC();
                    if (!canBeEaten('w',saveR,saveC,false)){
                        if (save->stillInCheck(saveR,saveC) &&
                            save != NULL) return true;
                        return false;
                    }
                    if ((*i)->getState() == NULL &&
                            gridCheck(saveR-1,saveC) &&
                            theGrid[saveR-1][saveC].getRank() == 'p'){
                        return false;
                    }
                    if ((*i)->getState() == NULL && gridCheck(saveR-2,saveC) &&
                            theGrid[saveR-2][saveC].getRank() == 'p' &&
                            theGrid[saveR-2][saveC].getState()->getFirst()){
                        return false;
                    }
                }
                return true;
            }
        }
    } else if (turn == 'w' && wk){
        int r = wk->getR();
        int c = wk->getC();
        if (canBeEaten('w',r,c)){
            isCheck = true;
            if (cantMoveKing(wk)){
                for (vector<Cell *>::iterator i = kingsave.begin(); i != kingsave.end() ; i++) {
                    int saveR = (*i)->getR();
                    int saveC = (*i)->getC();
                    if (!canBeEaten('b',saveR,saveC,false)){
                        if (save->stillInCheck(saveR,saveC) &&
                            save != NULL) return true;
                        return false;
                    }
                    if ((*i)->getState() == NULL &&
                            gridCheck(saveR+1,saveC) &&
                            theGrid[saveR+1][saveC].getRank() == 'P'){
                        return false;
                    }
                    if ((*i)->getState() == NULL && gridCheck(saveR+2,saveC) &&
                            theGrid[saveR+2][saveC].getRank() == 'P' &&
                            theGrid[saveR+2][saveC].getState()->getFirst()){
                        return false;
                    }
                }
                return true;
            }
        }
    }
    return false;
}

/*****allVectorsEmpty****
 * returns true if all vectors in black/white are empty
 ************************/
 bool Game::allVectorsEmpty(){
   if (bqueens.empty() && bbishops.empty() &&
      bknights.empty() && brooks.empty() &&
      bpawns.empty() && wqueens.empty() &&
      wbishops.empty() && wknights.empty() &&
      wrooks.empty() && wpawns.empty()){
    return true;
   } else {
     return false;
   }
 }

/*****updatePieces****
 * updates the list of possible edible locations on the board
 * for the pieces given and returns true if they are can eat something.
 ***********************/
bool Game::updatePieces(vector <Cell *> pieces){
  for (vector <Cell *>::iterator i = pieces.begin(); i != pieces.end(); i++){
      (*i)->addToEats();
      if (!(*i)->getEats().empty()) return true;
  }
  return false;
}

/*****updatePawns****
 * updates the list of possible edible locations on the board
 * for the pawns given and returns true if they are can eat something.
 ***********************/
bool Game::updatePawns(vector <Cell *> pawns){
  for (vector <Cell *>::iterator i = pawns.begin(); i != pawns.end(); i++){
    Cell * pawn = (*i);
    pawn->addToEats();
    char rank = pawn->getRank();
    int r = pawn->getR();
    int c = pawn->getC();
    if (rank == 'p'){
      if (!theGrid[r+1][c].getState()) {
        pawn->addAdd(&theGrid[r+1][c]);
        if (r == 1 && pawn->getFirst() &&
        !theGrid[r+2][c].getState()){
          pawn->addAdd(&theGrid[r+2][c]);
        }
      }
    } else if (rank == 'P'){
      if (!theGrid[r-1][c].getState()) {
        pawn->addAdd(&theGrid[r-1][c]);
        if (r == 6 && pawn->getFirst() &&
        !theGrid[r-2][c].getState()){
          pawn->addAdd(&theGrid[r-2][c]);
        }
      }
    }
    if (!pawn->getEats().empty()) return true;
  }
  return false;
}

/*****stalemateCheck***
 * returns true if it's a stalemate, false otherwise
 ***********************/
bool Game::stalemateCheck(){
    if (cantMoveKing(bk)){
        if(updatePieces(bqueens))return false;
        if(updatePieces(bbishops))return false;
        if(updatePieces(bknights))return false;
        if(updatePieces(brooks))return false;
        if(updatePawns(bpawns))return false;
    } else if (cantMoveKing(wk)){
        if(updatePieces(wqueens))return false;
        if(updatePieces(wbishops))return false;
        if(updatePieces(wknights))return false;
        if(updatePieces(wrooks))return false;
        if(updatePawns(wpawns))return false;
    } else {
        return false;
    }
    return true;
}




/****rookBlock*****
 * checks if a rook piece is blocked
 *******************/
bool Game::rookBlock(int r, int c, int dr, int dc){
    if ((r == dr) && (c < dc)){
        while(c != dc - 1){
            c++;
            if(theGrid[r][c].getState()){
                return false;
            }
        }
    } else if (r == dr){
        while(c != dc + 1){
            c--;
            if(theGrid[r][c].getState()){
                return false;
            }
        }
    } else if ((c == dc) && (r < dr)){
        while(r != dr - 1){
            r++;
            if (theGrid[r][c].getState()){
                return false;
            }
        }
    } else  if (c == dc){
        while(r != dr + 1){
            r--;
            if (theGrid[r][c].getState()){
                return false;
            }
        }
    } else {
        return false;
    }
    return true;
}

/***queenBlock***
 * checks if a queen piece is blocked. returns
 * true if not blocked, false if blocked
 *****************/
bool Game::queenBlock(int r, int c, int dr, int dc){
    // if destination is diagonal of the original position
    if (((r > dr ) && (c > dc )) ||
            ((r > dr ) && (c < dc)) ||
            ((r < dr) && (c > dc )) ||
            ((r < dr) && (c < dc))){
        return bishopBlock(r,c,dr,dc);
    }
    // if destination is on same row or same column
    if ((r == dr) || (c == dc)){
        return rookBlock(r,c,dr,dc);
    }
    return 0; // needs to return something for warning to go away..
}

/***bishopBlock***
 * checks if a bishop piece is blocked. returns
 * true if not blocked, false if blocked
 ******************/
bool Game::bishopBlock(int r, int c, int dr, int dc){
    // dest is bottom left of original
    // dr/dc + 1 is to not check actual dr/dc
    if ((r > dr + 1) && (c > dc + 1)){
        while ((r > dr + 1) && (c > dc + 1)){
            r--;
            c--;
            // if the path is blocked; it's not null
            if (theGrid[r][c].getState()){
                return false;
            }
        }
    } else if ((r > dr + 1) && (c + 1 < dc)){
        // dest is bottom right of original
        // +1 is to not check actual dr/dc
        while ((r > dr + 1) && (c + 1 < dc)){
            r--;
            c++;
            // if path is blocked; it's not null
            if (theGrid[r][c].getState()){
                return false;
            }
        }
    } else if ((r + 1 < dr) && (c >  dc + 1)){
        // dest is top left of original
        while ((r + 1 < dr) && (c >  dc + 1)){
            r++;
            c--;
            // if path is blocked, it's not null
            if (theGrid[r][c].getState()){
                return false;
            }
        }
    } else if ((r + 1 < dr) && (c + 1 < dc)){
        // dest is top right of original
        while ((r + 1 < dr) && (c + 1 < dc)){
            r++;
            c++;
            // if path is blocked, it's not null
            if (theGrid[r][c].getState()){
                return false;
            }
        }
    }

    return true;
}

/*****blockCheck******
 * checks if there's anything in the way of the move according
 * to what piece is being moved. returns true if there's nothing
 * blocking it, and false otherwise
 **********************/
bool Game::blockCheck(Cell *starting, int new_r, int new_c){
    char rank = starting->getRank();
    int r = starting->getR();
    int c = starting->getC();
    if (rank == 'R' || rank == 'r'){
        return rookBlock(r,c,new_r,new_c);
    } else if (rank == 'B' || rank == 'b'){
        return bishopBlock(r,c,new_r,new_c);
    } else if (rank == 'Q' || rank == 'q'){
        return queenBlock(r,c,new_r,new_c);
    }
    return true;
}

/******basicCheck*****
 * checks if there's anything blocking the starting piece
 * to the ending piece, if they're the same colour, if it'll
 * put king incheck, etc. Returns false if true if everything is o.k
 * (i.e legal, etc) false otherwise
**********************/
bool Game::basicCheck(int old_r, int old_c, int new_r, int new_c){
    // if the given coordinates are out of the board
    if (!gridCheck(new_r,new_c)){
        cout << "illegal move; location is not in the board" << endl;
        return false;
    }
    if (old_r == new_r && old_c == new_c){
        cout << "illegal move; you can't move it to the same spot" << endl;
        return false;
    }
    Piece *starting = theGrid[old_r][old_c].getState();
    Piece *ending = theGrid[new_r][new_c].getState();
    // if starting cell is empty, then do nothing
    if (!starting){
        cout << "illegal move; there's nothing there" << endl;
        return false;
    }
    // if it's the wrong colour
    if (starting->getColour() != turn){
        cout << "illegal move; not your colour" << endl;
        return false;
    }
    // if ending cell is filled, check if starting and ending pt are same colour
    if (ending && (starting->getColour() == ending->getColour())){
        cout << "illegal move; your own piece is there" << endl;
        return false;
    }
    //if its in check aftermove();
    if (theGrid[old_r][old_c].stillInCheck(new_r, new_c)){
        cout << "illegal move; will leave the king in check" << endl;
        return false;
    }
    if (!blockCheck(&theGrid[old_r][old_c], new_r, new_c)){
        cout << "illgal move; path is blocked" << endl;
        return false;
    }
    return true;
}

/****notify****
 * checks if the move is within board confines, and if it's a valid move
 * (i.e nothing blocking, valid for the piece) it moves the piece to the
 * new location
 ***************/
void Game::notify(int old_r, int old_c, int new_r, int new_c,char promo){
    // basic check checks if there's anything blocking it, if it's within board
    // confines, etc
    if (promo != 'A' && !basicCheck(old_r,old_c,new_r,new_c)){
        return;
    }
    Piece *ending = theGrid[new_r][new_c].getState();
    // if ending cell is filled and is not blocked, eat it!
    if (ending){
        char rank = theGrid[old_r][old_c].getRank();
        if (promo != '0' && promo != 'A' && (rank == 'P' || rank == 'p')){
            theGrid[old_r][old_c].eatPromotePiece(new_r,new_c,promo);
        } else if (rank == 'p' && new_r == 7){
            theGrid[old_r][old_c].eatPromotePiece(new_r,new_c,'q');
        } else if (rank == 'P' && new_r == 0){
            theGrid[old_r][old_c].eatPromotePiece(new_r,new_c,'Q');
        } else {
            theGrid[old_r][old_c].eatPiece(new_r,new_c);
        }
    } else { // if ending cell is not filled and not blocked, move to it
        char rank = theGrid[old_r][old_c].getRank();
        bool enpass = theGrid[old_r][old_c].getEnpass();
        bool first = theGrid[old_r][old_c].getFirst();
        int kcol = theGrid[old_r][old_c].getC();
        if (promo != '0' && promo != 'A' && (rank == 'P' || rank == 'p')) {
            theGrid[old_r][old_c].promotePiece(new_r,new_c,promo);
        } else if (rank == 'K' && first && ((new_r == 7 && new_c == 2) ||
                    (new_r == 7 && new_c == 6)) && kcol == 4){
            if (old_r == 7 && old_c == 4){
                theGrid[old_r][old_c].castlingPiece(new_r,new_c);
            } else {
                return;
            }
        } else if (rank == 'k' && first && ((new_r == 0 && new_c == 2) ||
                    (new_r == 0 && new_c == 6)) && kcol == 4){
            if (old_r == 0 && old_c == 4){
                theGrid[old_r][old_c].castlingPiece(new_r,new_c);
            } else {
                return;
            }
        } else if (rank == 'P' && enpass && old_c != new_c){
            // enpassant for white piece.
            char benpassrank = theGrid[new_r+1][new_c].getRank();
            if(benpassrank == 'p'){
                theGrid[old_r][old_c].enpassPiece(new_r,new_c);
            } else {
                return;
            }
        } else if (rank == 'p' && enpass && old_c != new_c){
            // enpassant for black piece.
            char wenpassrank = theGrid[new_r-1][new_c].getRank();
            if (wenpassrank == 'P'){
                theGrid[old_r][old_c].enpassPiece(new_r,new_c);
            } else {
                return;
            }
        } else {
            //break on promotion without anything.
            //normal move.
            if(rank == 'p' && new_r == 7){
                theGrid[old_r][old_c].promotePiece(new_r,new_c,'q');
            }else if (rank == 'P' && new_r == 0){
                theGrid[old_r][old_c].promotePiece(new_r,new_c,'Q');
            } else {
                theGrid[old_r][old_c].movePiece(new_r,new_c);
            }
        }
    }
}

/****hasPawn***
 * returns true if there is a pawn in
 * the first or last row; false o/w
***************/
bool Game::hasPawn(){
    int lrow = 0;
    int frow = 7;
    for (int i = 0; i < gridSize ; i++){
        if (theGrid[lrow][i].getState()){
            if (theGrid[lrow][i].getRank() == 'P' || theGrid[lrow][i].getRank() == 'p' ) {
                return true;
            }
        }
        if (theGrid[frow][i].getState()){
            if (theGrid[frow][i].getRank() == 'P' || theGrid[frow][i].getRank() == 'p' ) {
                return true;
            }
        }
    }
    return false;
}

/*****init****
 * initializes the game and the board
 **************/
void Game::init(GameNotification *GameNotification){
    clearGame();
    notification = GameNotification;
    theGrid = new Cell*[gridSize];
    for (int i = 0; i < gridSize; i++){
        theGrid[i] = new Cell[gridSize];
        for (int k = 0 ; k < gridSize; k++){
            theGrid[i][k] = Cell();
            theGrid[i][k].setCoords(i,k);
            theGrid[i][k].setGame(this);
        }
    }
}

/******init*****
 * puts the piece on the board
****************/
void Game::init(int r, int c, char piece){
    if (r <= gridSize && r > 0 &&
            c < gridSize && c >= 0){
        Piece *p = NULL;
        if (piece == 'k'){
            if (bk != NULL){
                bk->setStatemin();
            }
            p = new King(piece);
        } else if (piece == 'K'){
            if (wk != NULL){
                wk->setStatemin();
            }
            p = new King(piece);
        } else if (piece == 'q' || piece == 'Q'){
            p = new Queen(piece);
        } else if (piece == 'b' || piece == 'B'){
            p = new Bishop(piece);
        } else if (piece == 'n' || piece == 'N'){
            p = new Knight(piece);
        } else if (piece == 'r' || piece == 'R'){
            p = new Rook(piece);
        } else if (piece == 'p' || piece == 'P'){
            p = new Pawn(piece);
        }
        r--;
        theGrid[7-r][c].selfpush(piece);
        theGrid[7-r][c].setStateplus(p);
    }
}

/*****takeOff****
 * takes off whatever piece is on the
 * cell at location (r,c)
*****************/
void Game::takeOff(int r, int c){
    if (r <= gridSize && r > 0 &&
            c < gridSize && c >= 0){
        r--;
        theGrid[7-r][c].setStatemin();
    }
}

// sets the turn of the game
void Game::setTurn(char turn){
    this->turn = turn;
}

// returns the turn of the game
char Game::getTurn(){
    return turn;
}

// switches the turn of the game
void Game::switchTurn(){
    if (getTurn() == 'w'){
        setTurn('b');
    } else {
        setTurn('w');
    }
}

/*****canBeEaten*****
  returns true if the piece at (r,c) can be eaten, false otherwise
 ********************/
bool Game::canBeEaten(char turn, int r, int c, bool on){
    int originalR = r;
    int originalC = c;
    // dest is bottom left diagonal of original
    // goes to end of board
    if (on) kingsave.clear();
    while ((r < 7) && (c > 0)){
        r++;
        c--;
        if (theGrid[r][c].getState() == NULL){
            if (on) kingsave.push_back(&(theGrid[r][c]));
        } else {
            // if the path is blocked; it's not null
            char enemy = theGrid[r][c].getState()->getRank(); // getturn
            if ((turn == 'b') && ((enemy == 'Q') || (enemy == 'B'))){
                if (on) kingsave.push_back(&(theGrid[r][c]));
                save = &(theGrid[r][c]);
                return true;
            } else if ((turn == 'w') && ((enemy == 'q') || (enemy == 'b'))){
                if (on) kingsave.push_back(&(theGrid[r][c]));
                save = &(theGrid[r][c]);
                return true;
            } else {
                break;
            }
        }
    }
    if (on) kingsave.clear();
    r = originalR;
    c = originalC;
    // dest is bottom right diagonal of original
    // +1 is to not check actual dr/dc
    while ((r < 7) && (c < 7)){
        r++;
        c++;
        if (theGrid[r][c].getState() == NULL){
            if (on) kingsave.push_back(&(theGrid[r][c]));
        } else {
            // if path is blocked; it's not null
            char enemy = theGrid[r][c].getState()->getRank();
            if ((turn == 'b') && ((enemy == 'Q') || (enemy == 'B'))){
                if (on) kingsave.push_back(&(theGrid[r][c]));
                save = &(theGrid[r][c]);
                return true;
            } else if ((turn == 'w') && ((enemy == 'q') || (enemy == 'b'))){
                if (on) kingsave.push_back(&(theGrid[r][c]));
                save = &(theGrid[r][c]);
                return true;
            } else {
                break;
            }
        }
    }
    if (on) kingsave.clear();
    r = originalR;
    c = originalC;
    // dest is top left diagonal of original
    while ((r > 0) && (c > 0)){
        r--;
        c--;
        if (theGrid[r][c].getState() == NULL){
            if (on) kingsave.push_back(&(theGrid[r][c]));
        } else {
            // if path is blocked, it's not null
            char enemy = theGrid[r][c].getState()->getRank();
            if ((turn == 'b') && ((enemy == 'Q') || (enemy == 'B'))){
                if (on) kingsave.push_back(&(theGrid[r][c]));
                save = &(theGrid[r][c]);
                return true;
            } else if ((turn == 'w') && ((enemy == 'q') || (enemy == 'b'))){
                if (on) kingsave.push_back(&(theGrid[r][c]));
                save = &(theGrid[r][c]);
                return true;
            } else {
                break;
            }
        }
    }
    if (on) kingsave.clear();
    r = originalR;
    c = originalC;
    // dest is top right diagonal of original
    while ((r > 0) && (c < 7)){
        r--;
        c++;
        if (theGrid[r][c].getState() == NULL){
            if (on) kingsave.push_back(&(theGrid[r][c]));
        } else {
            // if path is blocked, it's not null
            char enemy = theGrid[r][c].getState()->getRank();
            if ((turn == 'b') && ((enemy == 'Q') || (enemy == 'B'))){
                if (on) kingsave.push_back(&(theGrid[r][c]));
                save = &(theGrid[r][c]);
                return true;
            } else if ((turn == 'w') && ((enemy == 'q') || (enemy == 'b'))){
                if (on) kingsave.push_back(&(theGrid[r][c]));
                save = &(theGrid[r][c]);
                return true;
            } else {
                break;
            }
        }
    }
    if (on) kingsave.clear();
    r = originalR;
    c = originalC;
    // dest is same column to the bottom of original
    while (r < 7){
        r++;
        if (theGrid[r][c].getState() == NULL){
            if (on) kingsave.push_back(&(theGrid[r][c]));
        } else {
            //if path is blocked, it's not null
            char enemy = theGrid[r][c].getState()->getRank();
            if ((turn == 'b') && ((enemy == 'Q') || (enemy == 'R'))){
                if (on) kingsave.push_back(&(theGrid[r][c]));
                save = &(theGrid[r][c]);
                return true;
            } else if ((turn == 'w') && ((enemy == 'q') || (enemy == 'r'))){
                if (on) kingsave.push_back(&(theGrid[r][c]));
                save = &(theGrid[r][c]);
                return true;
            } else {
                break;
            }
        }
    }
    if (on) kingsave.clear();
    r = originalR;
    c = originalC;
    // dest is same column to the top of original
    while (r > 0){
        r--;
        if (theGrid[r][c].getState() == NULL){
            if (on) kingsave.push_back(&(theGrid[r][c]));
        } else {
            char enemy = theGrid[r][c].getState()->getRank();
            if ((turn == 'b') && ((enemy == 'Q') || (enemy == 'R'))){
                if (on) kingsave.push_back(&(theGrid[r][c]));
                save = &(theGrid[r][c]);
                return true;
            } else if ((turn == 'w') && ((enemy == 'q') || (enemy == 'r'))){
                if (on) kingsave.push_back(&(theGrid[r][c]));
                save = &(theGrid[r][c]);
                return true;
            } else {
                break;
            }
        }
    }
    if (on) kingsave.clear();
    r = originalR;
    c = originalC;

    // dest is same row to the right of the original
    while (c < 7){
        c++;
        if (theGrid[r][c].getState() == NULL){
            if (on) kingsave.push_back(&(theGrid[r][c]));
        } else {
            char enemy = theGrid[r][c].getState()->getRank();
            if ((turn == 'b') && ((enemy == 'Q') || (enemy == 'R'))){
                if (on) kingsave.push_back(&(theGrid[r][c]));
                save = &(theGrid[r][c]);
                return true;
            } else if ((turn == 'w') && ((enemy == 'q') || (enemy == 'r'))){
                if (on) kingsave.push_back(&(theGrid[r][c]));
                save = &(theGrid[r][c]);
                return true;
            } else {
                break;
            }
        }
    }
    if (on) kingsave.clear();
    r = originalR;
    c = originalC;
    // dest is same row to the left of the original
    while (c > 0){
        c--;
        if (theGrid[r][c].getState() == NULL){
            if (on) kingsave.push_back(&(theGrid[r][c]));
        } else {
            char enemy = theGrid[r][c].getState()->getRank();
            if ((turn == 'b') && ((enemy == 'Q') || (enemy == 'R'))){
                if (on) kingsave.push_back(&(theGrid[r][c]));
                save = &(theGrid[r][c]);
                return true;
            } else if ((turn == 'w') && ((enemy == 'q') || (enemy == 'r'))){
                if (on) kingsave.push_back(&(theGrid[r][c]));
                save = &(theGrid[r][c]);
                return true;
            } else {
                break;
            }
        }
    }
    if (on) kingsave.clear();
    // horse
    r = originalR;
    c = originalC;
    if ((r + 1 < gridSize) && (c + 2 < gridSize) && theGrid[r+1][c+2].getState()){
        char enemy = theGrid[r+1][c+2].getState()->getRank();
        if ((turn == 'b') && (enemy == 'N')){
            if (on) kingsave.push_back(&(theGrid[r+1][c+2]));
            save = &(theGrid[r+1][c+2]);
            return true;
        } else if ((turn == 'w') && (enemy == 'n')){
            if (on) kingsave.push_back(&(theGrid[r+1][c+2]));
            save = &(theGrid[r+1][c+2]);
            return true;
        }
    }
    if (on) kingsave.clear();
    r = originalR;
    c = originalC;
    // horse
    if ((r + 2 < gridSize) && (c + 1 < gridSize) && theGrid[r+2][c+1].getState()){
        char enemy = theGrid[r+2][c+1].getState()->getRank();
        if ((turn == 'b') && (enemy == 'N')){
            if (on) kingsave.push_back(&(theGrid[r+2][c+1]));
            save = &(theGrid[r+2][c+1]);
            return true;
        } else if ((turn == 'w') && (enemy == 'n')){
            if (on) kingsave.push_back(&(theGrid[r+2][c+1]));
            save = &(theGrid[r+2][c+1]);
            return true;
        }
    }
    if (on) kingsave.clear();
    // horse
    r = originalR;
    c = originalC;
    if ((r - 1 >= 0) && (c - 2 >= 0) && theGrid[r-1][c-2].getState()){
        char enemy = theGrid[r-1][c-2].getState()->getRank();
        if ((turn == 'b') && (enemy == 'N')){
            if (on) kingsave.push_back(&(theGrid[r-1][c-2]));
            save = &(theGrid[r-1][c-2]);
            return true;
        } else if ((turn == 'w') && (enemy == 'n')){
            if (on) kingsave.push_back(&(theGrid[r-1][c-2]));
            save = &(theGrid[r-1][c-2]);
            return true;
        }
    }
    if (on) kingsave.clear();
    // horse
    r = originalR;
    c = originalC;
    if ((r - 2 >= 0) && (c - 1 >= 0) && theGrid[r-2][c-1].getState()){
        char enemy = theGrid[r-2][c-1].getState()->getRank();
        if ((turn == 'b') && (enemy == 'N')){
            if (on) kingsave.push_back(&(theGrid[r-2][c-1]));
            save = &(theGrid[r-2][c-1]);
            return true;
        } else if ((turn == 'w') && (enemy == 'n')){
            if (on) kingsave.push_back(&(theGrid[r-2][c-1]));
            save = &(theGrid[r-2][c-1]);
            return true;
        }
    }
    if (on) kingsave.clear();
    // horse
    r = originalR;
    c = originalC;
    if ((r - 1 >= 0) && (c + 2 < gridSize) && theGrid[r-1][c+2].getState()){
        char enemy = theGrid[r-1][c+2].getState()->getRank();
        if ((turn == 'b') && (enemy == 'N')){
            if (on) kingsave.push_back(&(theGrid[r-1][c+2]));
            save = &(theGrid[r-1][c+2]);
            return true;
        } else if ((turn == 'w') && (enemy == 'n')){
            if (on) kingsave.push_back(&(theGrid[r-1][c+2]));
            save = &(theGrid[r-1][c+2]);
            return true;
        }
    }
    if (on) kingsave.clear();
    // horse
    r = originalR;
    c = originalC;
    if ((r - 2 >= 0) && (c + 1 < gridSize) && theGrid[r-2][c+1].getState()){
        char enemy = theGrid[r-2][c+1].getState()->getRank();
        if ((turn == 'b') && (enemy == 'N')){
            if (on) kingsave.push_back(&(theGrid[r-2][c+1]));
            save = &(theGrid[r-2][c+1]);
            return true;
        } else if ((turn == 'w') && (enemy == 'n')){
            if (on) kingsave.push_back(&(theGrid[r-2][c+1]));
            save = &(theGrid[r-1][c+1]);
            return true;
        }
    }
    if (on) kingsave.clear();
    // horse
    r = originalR;
    c = originalC;
    if ((r + 1 < gridSize) && (c - 2 >= 0) && theGrid[r+1][c-2].getState()){
        char enemy = theGrid[r+1][c-2].getState()->getRank();
        if ((turn == 'b') && (enemy == 'N')){
            if (on) kingsave.push_back(&(theGrid[r+1][c-2]));
            save = &(theGrid[r+1][c-2]);
            return true;
        } else if ((turn == 'w') && (enemy == 'n')){
            if (on) kingsave.push_back(&(theGrid[r+1][c-2]));
            save = &(theGrid[r+1][c-2]);
            return true;
        }
    }
    if (on) kingsave.clear();
    // horse
    r = originalR;
    c = originalC;
    if ((r + 2 < gridSize) && (c - 1 >= 0) && theGrid[r+2][c-1].getState()){
        char enemy = theGrid[r+2][c-1].getState()->getRank();
        if ((turn == 'b') && (enemy == 'N')){
            if (on) kingsave.push_back(&(theGrid[r+2][c-1]));
            save = &(theGrid[r+2][c-1]);
            return true;
        } else if ((turn == 'w') && (enemy == 'n')){
            if (on) kingsave.push_back(&(theGrid[r+2][c-1]));
            save = &(theGrid[r+2][c-1]);
            return true;
        }
    }
    if (on) kingsave.clear();
    //Black Pawn
    r = originalR;
    c = originalC;
    if (turn == 'b'){
        if ((r + 1 < gridSize) && (c + 1 < gridSize) && theGrid[r+1][c+1].getState()){
            char enemy = theGrid[r+1][c+1].getState()->getRank();
            if (enemy == 'P'){
                if (on) kingsave.push_back(&(theGrid[r+1][c+1]));
                save = &(theGrid[r+1][c+1]);
                return true;
            }
        }
        if ((r + 1 < gridSize) && (c - 1 >= 0) && theGrid[r+1][c-1].getState()){
            char enemy = theGrid[r+1][c-1].getState()->getRank();
            if (enemy == 'P'){
                if (on) kingsave.push_back(&(theGrid[r+1][c-1]));
                save = &(theGrid[r+1][c-1]);
                return true;
            }
        }
    }
    if (on) kingsave.clear();
    //White Pawn
    r = originalR;
    c = originalC;
    if (turn == 'w'){
        if ((r - 1 >= 0) && (c + 1 < gridSize) && theGrid[r-1][c+1].getState()){
            char enemy = theGrid[r-1][c+1].getState()->getRank();
            if (enemy == 'p'){
                if (on) kingsave.push_back(&(theGrid[r-1][c+1]));
                save = &(theGrid[r-1][c+1]);
                return true;
            }
        }
        if ((r - 1 >= 0) && (c - 1 >= 0) && theGrid[r-1][c-1].getState()){
            char enemy = theGrid[r-1][c-1].getState()->getRank();
            if (enemy == 'p'){
                if (on) kingsave.push_back(&(theGrid[r-1][c-1]));
                save = &(theGrid[r-1][c-1]);
                return true;
            }
        }
    }
    if (on) kingsave.clear();
    // check for other king
    r = originalR;
    c = originalC;
    // bottom left
    if ((r - 1 >= 0) && (c - 1 >= 0) && theGrid[r-1][c-1].getState()){
        char enemy = theGrid[r-1][c-1].getState()->getRank();
        if (((turn == 'b') && (enemy == 'K')) || ((turn == 'w') && (enemy == 'k'))){
            if (on) kingsave.push_back(&(theGrid[r-1][c-1]));
            save = &(theGrid[r-1][c-1]);
            return true;
        }
    }
    if (on) kingsave.clear();
    // left
    if ((c - 1 >= 0) && theGrid[r][c-1].getState()){
        char enemy = theGrid[r][c-1].getState()->getRank();
        if (((turn == 'b') && (enemy == 'K')) || ((turn == 'w') && (enemy == 'k'))){
            if (on) kingsave.push_back(&(theGrid[r][c-1]));
            save = &(theGrid[r][c-1]);
            return true;
        }
    }
    if (on) kingsave.clear();
    // top left
    if ((r + 1 < gridSize) && (c - 1 >= 0) && theGrid[r+1][c-1].getState()){
        char enemy = theGrid[r+1][c-1].getState()->getRank();
        if (((turn == 'b') && (enemy == 'K')) || ((turn == 'w') && (enemy == 'k'))){
            if (on) kingsave.push_back(&(theGrid[r+1][c-1]));
            save = &(theGrid[r+1][c-1]);
            return true;
        }
    }
    if (on) kingsave.clear();
    // top
    if ((r + 1 < gridSize)  && theGrid[r+1][c].getState()){
        char enemy = theGrid[r+1][c].getState()->getRank();
        if (((turn == 'b') && (enemy == 'K')) || ((turn == 'w') && (enemy == 'k'))){
            if (on) kingsave.push_back(&(theGrid[r+1][c]));
            save = &(theGrid[r+1][c-1]);
            return true;
        }
    }
    if (on) kingsave.clear();
    // top right
    if ((r + 1 < gridSize) && (c + 1 < gridSize) && theGrid[r+1][c+1].getState()){
        char enemy = theGrid[r+1][c+1].getState()->getRank();
        if (((turn == 'b') && (enemy == 'K')) || ((turn == 'w') && (enemy == 'k'))){
            if (on) kingsave.push_back(&(theGrid[r+1][c+1]));
            save = &(theGrid[r+1][c+1]);
            return true;
        }
    }
    if (on) kingsave.clear();
    // right
    if ((c + 1 < gridSize) && theGrid[r][c+1].getState()){
        char enemy = theGrid[r][c+1].getState()->getRank();
        if (((turn == 'b') && (enemy == 'K')) || ((turn == 'w') && (enemy == 'k'))){
            if (on) kingsave.push_back(&(theGrid[r][c+1]));
            save = &(theGrid[r][c+1]);
            return true;
        }
    }
    if (on) kingsave.clear();
    // bottom right
    if ((r - 1 >= 0) && (c + 1 < gridSize) && theGrid[r-1][c+1].getState()){
        char enemy = theGrid[r-1][c+1].getState()->getRank();
        if (((turn == 'b') && (enemy == 'K')) || ((turn == 'w') && (enemy == 'k'))){
            if (on) kingsave.push_back(&(theGrid[r-1][c+1]));
            save = &(theGrid[r-1][c+1]);
            return true;
        }
    }
    if (on) kingsave.clear();
    // bottom
    if ((r + 1 < gridSize) && theGrid[r+1][c].getState()){
        char enemy = theGrid[r+1][c].getState()->getRank();
        if (((turn == 'b') && (enemy == 'K')) || ((turn == 'w') && (enemy == 'k'))){
            if (on) kingsave.push_back(&(theGrid[r+1][c]));
            save = &(theGrid[r+1][c]);
            return true;
        }
    }
    if (on) kingsave.clear();
    return false;
}


/*****enpassReset*****
 * updates the enpassant value for all
 * the pawns
*********************/
void Game::enpassReset(){
    if (pass > 0 && pass < 4){
        if (turn == 'w'){
            for (vector <Cell *>::iterator i = wpawns.begin(); i != wpawns.end(); i++){
                (*i)->getState()->setEnpass(false);
            }
        } else if (turn == 'b'){
            for (vector <Cell *>::iterator i = bpawns.begin(); i != bpawns.end(); i++){
                (*i)->getState()->setEnpass(false);
            }
        }
    }
    if(pass < 3){
        pass++;
    }
}

/*****setEnPassant*****
 * sets the enpassant value to 0; while pass is less than 3,
* enpassant is possible
************************/
void Game::setEnPassant(){
    pass = 0;
}

/*****gridCheck****
 * returns true if it's within the board
 * false otherwise
*******************/
bool Game::gridCheck(int r,int c){
    return (r < gridSize && r >= 0 && c < gridSize && c >= 0);
}

/*****computerMove*****
 * implements the AI's move
 ***********************/
void Game::computerMove(Computer *comp){
    string move = comp->getMove();
    Cell *prev = comp->getPrev();
    int r = prev->getR();
    int c = prev->getC();
    stringstream iss(move);
    int new_r, new_c;
    iss >> new_r; //assuming they are integers; will be bad if they are not ints
    iss >> new_c;
    notify(r,c,new_r,new_c,'A');
}


void Game::testKingsave(bool on){
  if (on){
    cout << "KING IN CHECK COOR: ";
    if (kingsave.empty()) cout << "EMPTY FUCK!" << endl;

    for (vector<Cell *>::iterator i = kingsave.begin(); i != kingsave.end() ; i++) {

        cout << (*i)->getR() << "|"<< (*i)->getC()  << ". ";
    }
    cout << endl;
  }
}
