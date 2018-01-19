#include "computer.h"
#include <string>
#include <cstdlib>    
#include <ctime>
#include <iostream>
#include <vector>
#include <sstream>

using namespace std;

Player *Computer::createWhiteAi(char c, int i, Game *game){
   if (!white) { // if instance is not set, set it  
       white = new Computer(c,i,game);  
  } //if  
  return white;   
} 

Player *Computer::createBlackAi(char c, int i, Game *game) {
 if (!black) { // if instance is not set, set it
    black = new Computer(c,i,game);
    } //if
    return black;
}


// computer ctor
Computer::Computer(char c, int i, Game *game) :Player(c,false,true),game(game),
    prevMove(NULL),currentBestMove(NULL){
        this->level = i;
    }

/******getMove******
 * gets the move depending on
 * which level the AI is
 ********************/
string Computer::getMove(){
    cout << "♪ヽ( ⌒ o ⌒)人(⌒ - ⌒ )v ♪ ~ Computer is making a move ~ ᕙ(^▿^-ᕙ)┗(＾0＾)┓"<< endl;
    if (level == 1){
        level1();
    } else if (level == 2){
        level2();
    } else if (level == 3){
        level3();
    } else if (level == 4){
        level4();
    }
    int new_r = currentBestMove->getR();
    int new_c = currentBestMove->getC();
    stringstream ss;
    ss << new_r;
    string rr = ss.str();
    stringstream pp;
    pp << new_c;
    string cc = pp.str();
    string move = rr + " " + cc;
    return move;
}

/****getP****
 * provides a random piece from a vector.
 * returns a cell that is randomly picked
 *******************/
Cell * Computer::getP(vector<Cell *> piece){
    srand(time(NULL));
    int size = piece.size();
    if (size == 0) return NULL;
    int cell = rand() % size;
    return piece.at(cell);
}

/****getPiece****
 * provides a piece at random.
 * returns a cell that is randomly picked
 *******************/
int Computer::getPiece(int n){
    srand (time(NULL));
    prevMove = NULL;
    while (prevMove == NULL){
        int piece = rand() % 6;
        while(piece == n){
            piece = rand() % 6;
        }
        n = piece;

        if (piece == 0){
            if (getColour() == 'w'){
                prevMove = game->wk;
            } else {
                prevMove = game->bk;
            }
        } else if (piece == 1){
            if (getColour() == 'w'){
                prevMove = getP(game->wqueens);
            } else {
                prevMove = getP(game->bqueens);
            }
        } else if (piece == 2){ 
            if (getColour() == 'w'){
                prevMove = getP(game->wbishops);
            } else {
                prevMove = getP(game->bbishops);
            }
        } else if (piece == 3){
            if (getColour() == 'w'){
                prevMove = getP(game->wknights);
            } else {
                prevMove = getP(game->bknights);
            }
        } else if (piece == 4){
            if (getColour() == 'w'){
                prevMove = getP(game->wrooks);
            } else {
                prevMove = getP(game->brooks);
            }
        } else {
            if (getColour() == 'w'){
                prevMove = getP(game->wpawns);
            } else {
                prevMove = getP(game->bpawns);
            }
        }
    }
    return n;
}

/****pawnMove****
 * Add the possible move of a pawn in the possibleEat Array in a Cell.
 *******************/
void Computer::pawnMove(){
    char rank = prevMove->getRank();
    int r = prevMove->getR();
    int c = prevMove->getC();
    Cell * cur = prevMove;
    if (rank == 'p'){
        if (!game->theGrid[r+1][c].getState()) {
            cur->addAdd(&game->theGrid[r+1][c]);
            if (r == 1 && cur->getFirst() && 
                    !game->theGrid[r+2][c].getState()){
                cur->addAdd(&game->theGrid[r+2][c]);
            }
        }
    } else if (rank == 'P'){
        if (!game->theGrid[r-1][c].getState()) { 
            cur->addAdd(&game->theGrid[r-1][c]);
            if (r == 6 && cur->getFirst() && 
                    !game->theGrid[r-2][c].getState()){
                cur->addAdd(&game->theGrid[r-2][c]);
            }
        }
    }
}

/****castlingMove****
 * Add the castlling move to the king.
 *******************/
void Computer::castlingMove(){
    char rank = prevMove->getRank();
    bool first = prevMove->getState()->getFirst();
    int r = prevMove->getR();
    int c = prevMove->getC();
    Cell * cur = prevMove;
    if ((rank == 'k' || rank == 'K') && first && c == 4 && (r == 0 || r == 7)){
        if ((c+2 < gridSize && c+3 < gridSize) && !game->theGrid[r][c+2].getState() 
                && game->blockCheck(&game->theGrid[r][c+3], r, c)){
            char rrank = game->theGrid[r][c+3].getRank();
            int rfirst = game->theGrid[r][c+3].getFirst();
            if (rfirst && (rrank == 'r' || rrank == 'R')){
                cur->addAdd(&game->theGrid[r][c+2]);
            }
        }
        if ((c-2 >= 0 && c-4 >= 0) && !game->theGrid[r][c-2].getState() 
                && game->blockCheck(&game->theGrid[r][c-4], r, c)){
            int rrank = game->theGrid[r][c-4].getRank();
            int rfirst = game->theGrid[r][c-4].getFirst();
            if (rfirst && (rrank == 'r' || rrank == 'R')){
                cur->addAdd(&game->theGrid[r][c-2]);
            }
        }
    } 
}

// lvl1 AI
void Computer::level1(bool level4){
    bool gettingMove = true;
    srand (time(NULL));
    int n = 6;
    int off = true;
    int size = -1;
    int lvl4break = 0;
    while (gettingMove){
        if (off && game->isCheck){
            if (getColour() == 'w'){
                prevMove = game->wk;
            } else {
                prevMove = game->bk;
            }
        }
        // while there are no possible moves for the random piece
        while (size <= 0){
            if (off && game->isCheck){
                off = false;
            } else {
                n = getPiece(n);
            }

            prevMove->addToEats();
            if (!game->isCheck) castlingMove();
            pawnMove();
            size = prevMove->getEats().size(); //reset the size
        }
        // while your vector is not empty
        if (level4){
            lvl4break++;
            if (lvl4break >= 10) level4 = false;
        }
        while (size > 0){

            int move = rand() % size; // get a random number
            currentBestMove = prevMove->getEats().at(move);

            int new_r = currentBestMove->getR();
            int new_c = currentBestMove->getC();
            // if it's a valid move, and there's nothing blocking it that's the same colour,
            // and it won't put your king in check
            if (!prevMove->stillInCheck(new_r,new_c)){
                // if it's a lvl4 AI, make sure it doesn't move to somewhere it can be eaten
                if (level4 && inDanger(prevMove,new_r,new_c)){
                    // problem: what if there are no moves that it can't be eaten? if vector is empty,
                    // then just level1(), not level1(level4)
                    prevMove->eraseErase(move);
                    size = prevMove->getEats().size();
                    continue;
                }
                gettingMove = false;
                break;
            }
            prevMove->eraseErase(move);
            size = prevMove->getEats().size();
        }
    }
}

// level 2 AI
void Computer::level2(){
   if (getColour() == 'w'){
        level2White();
    } else {
        level2Black();
    }
}

// level 3 AI
void Computer::level3(){
    if (getColour() == 'w'){
        level3White();
    } else {
        level3Black();
    }
}


/*******inDanger*****
 * returns true if the pice is in danger of being eaten at new_r, new_c
 *********************/
bool Computer::inDanger(Cell *piece, int new_r, int new_c){
    int r = piece->getR();
    int c = piece->getC();
    char turn = getColour();
    Piece *start = piece->getState();
    game->theGrid[r][c].tempSetState(NULL); // if I moved there, nothing in my cell
    // if after the move, it can be eaten
    if (game->canBeEaten(turn, new_r, new_c, false)){
        // move it back
        game->theGrid[r][c].tempSetState(start);
        return true; // I am in danger
    }
    // move it back
    game->theGrid[r][c].tempSetState(start);
    return false;
}


/*******canEat*******
 * returns true if the piece can eat the other side's king
 * and false otherwise
 ***********************/
bool Computer::canEat(Cell *piece, bool lvl4){ 
    //srand (time(NULL));

    piece->addToEats();
    int size = piece->getEats().size();
    prevMove = piece;
    for (int i = 0; i < size; i++){
        int new_r = piece->getEats().at(i)->getR();
        int new_c = piece->getEats().at(i)->getC();        

        if (piece->getEats().at(i)->getState() && !piece->stillInCheck(new_r,new_c)){
            currentBestMove = piece->getEats().at(i);
            // if new_r/new_c is a location where the piece can be eaten 
            if (lvl4 && inDanger(piece,new_r,new_c)){
            return false; // can't eat without being in danger
            }
            return true;
        }
    }
    return false;
}

/*******canCheck*******
 * returns true if the piece can check the other side's king
 * and false otherwise
 ***********************/
bool Computer::canCheck(Cell *piece, bool lvl4){
    srand (time(NULL));
    piece->addToEats();
    int size = piece->getEats().size();
    prevMove = piece;

    while (size > 0){
        int move = rand() % size;

        int new_r = piece->getEats().at(move)->getR();
        int new_c = piece->getEats().at(move)->getC();
        // if new_r/new_c is a location where the piece can
        // be eaten
        if (lvl4 && inDanger(prevMove, new_r, new_c)){
            return false; // can't check without being in danger
        }
        game->switchTurn();
        // if the change makes the other king in check, and it doesn't make your king in check
        if (piece->stillInCheck(new_r,new_c)){
            game->switchTurn();
            if (!piece->stillInCheck(new_r,new_c)){
                currentBestMove = piece->getEats().at(move);
                return true;
            }
            game->switchTurn();
        }
        game->switchTurn();
        prevMove->eraseErase(move);
        size = prevMove->getEats().size();
    }
    return false;
}

// level2 AI for white pieces
void Computer::level2White(bool level4){
    if (canDie(game->wk,level4)){
        return;
    }
    // iterate over pawns; can a pawn eat anything?
    for (vector <Cell *>::iterator i = game->wpawns.begin(); i != game->wpawns.end(); i++){
        if (canEat(*i,level4)) {
            return;
        }
    }
    // over knights
    for (vector <Cell *>::iterator i = game->wknights.begin(); i != game->wknights.end(); i++){
        if (canEat(*i, level4)) {
            return;
        }
    }
    // over rooks
    for (vector <Cell *>::iterator i = game->wrooks.begin(); i != game->wrooks.end(); i++){
        if (canEat(*i, level4)) {
            return;
        }
    }
    // over bishops
    for (vector <Cell *>::iterator i = game->wbishops.begin(); i != game->wbishops.end(); i++){
        if (canEat(*i, level4)) {
            return;
        }
    }
    // over queens
    for (vector <Cell *>::iterator i = game->wqueens.begin(); i!= game->wqueens.end(); i++){
        if (canEat(*i, level4)) {
            return;
        }
    }
    // NOTHING IT CAN EAT...ANYTHING IT CAN CHECK? 
    // iterate over pawns; can any of them put the other king in check?
    for (vector <Cell *>::iterator i = game->wpawns.begin(); i != game->wpawns.end(); i++){
        if (canCheck(*i, level4)){
            return;
        }
    }
    // over knights
    for (vector <Cell *>::iterator i = game->wknights.begin(); i != game->wknights.end(); i++){
        if (canCheck(*i, level4)){
            return;
        }
    }
    // over rooks
    for (vector <Cell *>::iterator i = game->wrooks.begin(); i != game->wrooks.end(); i++){
        if (canCheck(*i, level4)){
            return;
        }
    }
    // over bishops
    for (vector <Cell *>::iterator i = game->wbishops.begin(); i != game->wbishops.end(); i++){
        if (canCheck(*i, level4)){
            return;
        }
    }
    // over queens
    for (vector <Cell *>::iterator i = game->wqueens.begin(); i!= game->wqueens.end(); i++){
        if (canCheck(*i, level4)){
            return;
        }
    }
    // can king eat anything?
    Cell *piece = game->wk;
    piece->addToEats();
    int size = piece->getEats().size();
    for (int i = 0; i < size; i++){
        int new_r = piece->getEats().at(i)->getR();
        int new_c = piece->getEats().at(i)->getC();
        if (piece->getEats().at(i)->getState() && !piece->stillInCheck(new_r,new_c)){
            currentBestMove = piece->getEats().at(i);
            prevMove = piece;
            return;
        }
    }
    level1(level4);
}

// level2 AI for black pieces
void Computer::level2Black(bool level4){
    // is the king in check? save it
    if (canDie(game->bk,level4)){
        return;
    }
    // iterate over pawns; can any of them eat anything?
    for (vector <Cell *>::iterator i = game->bpawns.begin(); i != game->bpawns.end(); i++){
        if (canEat(*i,level4)) {
            return;
        }
    }
    // over knights
    for (vector <Cell *>::iterator i = game->bknights.begin(); i != game->bknights.end(); i++){
        if (canEat(*i, level4)) {
            return;
        }
    }
    // over rooks
    for (vector <Cell *>::iterator i = game->brooks.begin(); i != game->brooks.end(); i++){
        if (canEat(*i, level4)){
            return;
        }
    }
    // over bishops
    for (vector <Cell *>::iterator i = game->bbishops.begin(); i != game->bbishops.end(); i++){
        if (canEat(*i, level4)) {
            return;
        }
    }
    // over queens
    for (vector <Cell *>::iterator i = game->bqueens.begin(); i!= game->bqueens.end(); i++){
        if (canEat(*i, level4)) {
            return;
        }
    }
    // NOTHING TO EAT...ANYTHING TO CHECK?
    // iterate over pawns; can any of them put the other king in check?
    for (vector <Cell *>::iterator i = game->bpawns.begin(); i != game->bpawns.end(); i++){
        if (canCheck(*i,level4)){
            return;
        }
    }
    // over knights
    for (vector <Cell *>::iterator i = game->bknights.begin(); i != game->bknights.end(); i++){
        if (canCheck(*i, level4)){
            return;
        }
    }
    // over rooks
    for (vector <Cell *>::iterator i = game->brooks.begin(); i != game->brooks.end(); i++){
        if (canCheck(*i, level4)){
            return;
        }
    }
    // over bishops
    for (vector <Cell *>::iterator i = game->bbishops.begin(); i != game->bbishops.end(); i++){
        if (canCheck(*i, level4)){
            return;
        }
    }
    // over queens
    for (vector <Cell *>::iterator i = game->bqueens.begin(); i!= game->bqueens.end(); i++){
        if (canCheck(*i, level4)){
            return;
        }
    }
    // can the king eat anything?
    Cell * piece = game->bk;
    piece->addToEats();
    int size = piece->getEats().size();
    for (int i = 0; i < size; i++){
        int new_r = piece->getEats().at(i)->getR();
        int new_c = piece->getEats().at(i)->getC();
        if (piece->getEats().at(i)->getState() && !piece->stillInCheck(new_r,new_c)){
            currentBestMove = piece->getEats().at(i);
            prevMove = piece;
            return;
        }
    }
    level1(level4);
}

/****canDie****
 * can the piece die? if it can, move it
 ***************/
bool Computer::canDie(Cell *piece, bool lvl4){
    srand (time(NULL));
    if (!inDanger(piece,piece->getR(),piece->getC())){
        prevMove = piece;
        prevMove->addToEats();
        pawnMove();

        // if it's the lvl4 AI, it will escape by eating if it can
        if (lvl4 && canEat(piece,lvl4)){
            return true;
        }
        // if lvl 4 AI, will escape by putting other king in check
        if (lvl4 && canCheck(piece,lvl4)){
            return true;
        }

        int size = prevMove->getEats().size();
        while (size > 0){
            int move = rand() % size;

            int new_r = piece->getEats().at(move)->getR();
            int new_c = piece->getEats().at(move)->getC();
            //1)if the possible cell doesnt put me in check
            //2)and if by moving there doest put my king in check
            if (!inDanger(prevMove,new_r,new_c) && 
                !prevMove->stillInCheck(new_r,new_c)){
                currentBestMove = piece->getEats().at(move);
                return true;
            }
            // if it can still be eaten or king's in check, no go; put the piece back
            prevMove->eraseErase(move);
            size = prevMove->getEats().size();
        }
    }
    return false;
}

// level3 AI for white pieces
void Computer::level3White(bool level4){
    // king
    if (canDie(game->wk, level4)){
        return;
    }
    // over queens
    for (vector <Cell *>::iterator i = game->wqueens.begin(); i!= game->wqueens.end(); i++){
        if (canDie(*i, level4)) {
            return;
        } 
    }

    // over bishops
    for (vector <Cell *>::iterator i = game->wbishops.begin(); i != game->wbishops.end(); i++){
        if (canDie(*i, level4)) {
            return;
        } 
    }


    // over rooks
    for (vector <Cell *>::iterator i = game->wrooks.begin(); i != game->wrooks.end(); i++){
        if (canDie(*i, level4)) {
            return;
        } 
    }

    // over knights
    for (vector <Cell *>::iterator i = game->wknights.begin(); i != game->wknights.end(); i++){
        if (canDie(*i, level4)) {
            return;
        } 
    }
    // ove pawns 
    for (vector <Cell *>::iterator i = game->wpawns.begin(); i != game->wpawns.end(); i++){
        if (canDie(*i, level4)) {
            return;
        } 
    }
    level2White(level4);
}

// level3 AI for black pieces
void Computer::level3Black(bool level4){
    // king
    
    if (canDie(game->bk, level4)){
        return;
    }
    // over queens
    for (vector <Cell *>::iterator i = game->bqueens.begin(); i!= game->bqueens.end(); i++){
        if (canDie(*i, level4)) {
            return;
        } 
    } 

    // over bishops
    for (vector <Cell *>::iterator i = game->bbishops.begin(); i != game->bbishops.end(); i++){
        if (canDie(*i, level4)) {
            return;
        } 
    }

    // over rooks
    for (vector <Cell *>::iterator i = game->brooks.begin(); i != game->brooks.end(); i++){
        if (canDie(*i, level4)) {
            return;
        } 
    }


    // over knights
    for (vector <Cell *>::iterator i = game->bknights.begin(); i != game->bknights.end(); i++){
        if (canDie(*i, level4)) {
            return;
        } 
    }

    //over pawns
    for (vector <Cell *>::iterator i = game->bpawns.begin(); i != game->bpawns.end(); i++){
        if (canDie(*i, level4)) {
            return;
        } 
    }
    level2Black(level4);
}

// level4 AI; will change computer's moves
void Computer::level4(){
    if (getColour() == 'w'){
        level4White();
    } else {
        level4Black();
    }
}

// level4 AI for white pieces
void Computer::level4White(){
    level3White(true);
}

// level4 AI for black pieces
void Computer::level4Black(){
    level3Black(true);
}

// getter for prev move
Cell *Computer::getPrev(){
    return prevMove;
}
