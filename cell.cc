#include <string>
#include <iostream>
#include <vector>
#include "cell.h"
#include "piece.h"

using namespace std;

// cell ctor
Cell::Cell():state(NULL){}

// cell dtor
Cell::~Cell() {
    delete state;
}

/****withinBoard******
 * returns true if (r,c) is
 * within the board
 **********************/
bool Cell::withinBoard(int r,int c){
    return (r < gridSize && r >= 0 && c < gridSize && c >= 0);
}

/****getR****
 * returns r of cell
 *************/
int Cell::getR(){
    return r;
}

/****getC****
 * returns c of cell
 ************/
int Cell::getC(){
    return c;
}

/*****getRank*****
 * if there is a piece on the cell, it will return
 * its rank. Otherwise, it will return however it is
 * represented on the text display
 ******************/
char Cell::getRank(){
    if (state != NULL){
        return state->getRank();
    } else {
        if ( r % 2 == 0 && c % 2 == 0){
            return ' ';
        }else if (r % 2 != 0 && c % 2 != 0){
            return ' ';
        }else  {
            return '_';
        }
    }
}

/******getColour****
 * returns the colour of the piece on the cell, and
 * '0' otherwise; should always be called on a cell
 * with a piece
 ********************/
char Cell::getColour(){
    if (state != NULL){
        return state->getColour();
    } else {
        return '0';
    }
}

/******getEnpass*****
 * returns the piece's enpass state, or false
 * if there is no piece
 *********************/
bool Cell::getEnpass(){
    if (state != NULL){
        return state->getEnpass();
    }
    return false;
}

/*****getFirst****
 * returns whether or not it's the first move
 * of the piece
******************/
bool Cell::getFirst(){
    if (state != NULL){
        return state->getFirst();
    } else {
        return false;
    }
}

/***getState***
 * returns the state of the cell
 ***************/
Piece *Cell::getState(){
    return state;
}

/****tempSetState***
 * temporarily sets the state of the
 * cell to piece without updating pointers, etc
 ********************/
void Cell::tempSetState(Piece *piece){
    state = piece;
}

/****movePiece****
 * if it's a valid move for the piece being moved
 * and it's not blocked, move the piece!
 ******************/
void Cell::movePiece(int new_r, int new_c){
    Piece *temp = this->state;
    char rank = this->getRank();
    char colour = this->getColour();
    bool first = temp->getFirst();
    if (temp->move(r,c,new_r,new_c)){
        // if it's the piece's first move and it's a pawn
        if (first && (rank == 'P' || rank == 'p')){
            // check if the spot right of it is in the board
            if (withinBoard(new_r,new_c+1)){
                char pawnrank = game->theGrid[new_r][new_c+1].getRank();
                char pawncolour = game->theGrid[new_r][new_c+1].getColour();
                if ((pawnrank == 'p' || pawnrank == 'P') && pawncolour != colour){
                    game->theGrid[new_r][new_c+1].state->setEnpass(true);
                    game->setEnPassant();
                }
            }
            // check if the spot left of it is in the board
            if (withinBoard(new_r,new_c-1)){
                char pawnrank = game->theGrid[new_r][new_c-1].getRank();
                char pawncolour = game->theGrid[new_r][new_c-1].getColour();
                if ((pawnrank == 'p' || pawnrank == 'P') && pawncolour != colour){
                    game->theGrid[new_r][new_c-1].state->setEnpass(true);
                    game->setEnPassant();
                }
            }
        }
        temp->setEnpass(false);
        temp->setFirst(false);
        // if this point has been reached and the king is in check, then the move is valid
        // and the king will not be in check
        game->isCheck = false;
        game->theGrid[new_r][new_c].setStateplus(temp);
        this->state = NULL;
        game->switchTurn();
        notifyGame();
    }
}

/*******eatPiece******
 * if it's a valid move for the piece being moved
 * move the piece and eat
 * whatever was there!
 **********************/
void Cell::eatPiece(int new_r, int new_c){
    Piece *temp = this->state;
    if (temp->eat(r,c,new_r,new_c)){
        temp->setEnpass(false);
        temp->setFirst(false);
        game->isCheck = false;
        game->theGrid[new_r][new_c].setStatemin();
        game->theGrid[new_r][new_c].setStateplus(temp);
        this->state = NULL;
        // change the turn
        game->switchTurn();
        notifyGame();
    }
}

/*********enPassPiece******
 * implements the enPassant move in chess
 ***************************/
void Cell::enpassPiece(int new_r, int new_c){
    Piece *temp = this->state;
    char rank = this->getRank();
    if (temp->eat(r,c,new_r,new_c)){
        temp->setEnpass(false);
        temp->setFirst(false);
        if (rank == 'p'){
            game->theGrid[new_r-1][new_c].setStatemin();
        } else {
            game->theGrid[new_r+1][new_c].setStatemin();
        }
        game->theGrid[new_r][new_c].setStateplus(temp);
        game->isCheck = false;
        this->state = NULL;
        game->switchTurn();
        notifyGame();
    }
}

/*********eatPromotePiece*******
 * promotes a pawn when it eats something
 * in the last row
 ********************************/
void Cell::eatPromotePiece(int new_r, int new_c, char promo){
    Piece *temp = this->state;
    char rank = this->getRank();
    if (temp->eat(r,c,new_r,new_c)){
        if (rank == 'p'){
            if (promo == 'q' || promo == 'n' || promo == 'b' || promo == 'r'){
                game->isCheck = false;
                setStatemin();
                game->theGrid[new_r][new_c].setStatemin();
                game->init(8-new_r,new_c,promo);
                game->switchTurn();
                notifyGame();
            } else {
                return;
            }
        } else if (rank == 'P'){
            if (promo == 'Q' || promo == 'N' || promo == 'B' || promo == 'R'){
                game->isCheck = false;
                setStatemin();
                game->theGrid[new_r][new_c].setStatemin();
                game->init(8-new_r,new_c,promo);
                game->switchTurn();
                notifyGame();
            } else {
                return;
            }
        }
    }
}

/******promotePiece*****
 * promotes the pawn when it moves into the last
 * row
 ************************/
void Cell::promotePiece(int new_r, int new_c, char promo){
    Piece *temp = this->state;
    char rank = this->getRank();
    if (temp->move(r,c,new_r,new_c)){
        if (rank == 'p' && (new_r == r+1 && new_c == c)){
            if (promo == 'q' || promo == 'n' || promo == 'b' || promo == 'r'){
                game->isCheck = false;
                setStatemin();
                game->init(8-new_r,new_c,promo);
                game->switchTurn();
                notifyGame();
            } else {
                return;
            }
        } else if (rank == 'P' && (new_r == r-1 && new_c == c)){
            if (promo == 'Q' || promo == 'N' || promo == 'B' || promo == 'R'){
                game->isCheck = false;
                setStatemin();
                game->init(8-new_r,new_c,promo);
                game->switchTurn();
                notifyGame();
            } else {
                return;
            }
        }
    }
}

/*********castingPiece*******
 * if a king's first move is to move towards
 * a rook that hasn't moved yet, it castles the
 * king
 ****************************/
void Cell::castlingPiece(int new_r, int new_c){
    Piece *temp = this->state;
    bool tempf = temp->getFirst();
    if (tempf && new_c == c-2){
        char rrank = game->theGrid[r][0].getRank();
        bool first = game->theGrid[r][0].getFirst();
        char colour = game->theGrid[r][0].getColour();
        if ((rrank == 'R' || rrank == 'r') && first &&
                game->theGrid[r][0].game->blockCheck(&(game->theGrid[r][0]),r,c) &&
                colour == getColour()){
            game->theGrid[r][0].movePiece(r,3);
            temp->setEnpass(false);
            temp->setFirst(false);
            game->isCheck = false;
            game->theGrid[new_r][new_c].setStateplus(temp);
            this->state = NULL;
            notifyGame();
        } else {
            return;
        }
    } else if (tempf && new_c == c+2){
        char rrank = game->theGrid[r][7].getRank();
        bool first = game->theGrid[r][7].getFirst();
        char colour = game->theGrid[r][7].getColour();
        if ((rrank == 'R' || rrank == 'r') && first  &&
                game->theGrid[r][7].game->blockCheck(&(game->theGrid[r][7]),r,c) &&
                colour == getColour()){
            game->theGrid[r][7].movePiece(r,5);
            temp->setEnpass(false);
            temp->setFirst(false);
            game->isCheck = false;
            game->theGrid[new_r][new_c].setStateplus(temp);
            this->state = NULL;
            notifyGame();
        } else {
            return;
        }
    }
}

/****clearEats****
 * clears the cell's vector of possible
 * cells to eat
 ******************/
void Cell::clearEats(){
    possibleEats.clear();
}

/*****setStateplus*****
 * sets the cell's state to
 * piece p, and updates the game's
 * pointer to pieces
 ***********************/
void Cell::setStateplus(Piece *p){
    // if there's something there; should only be reached in AI
    if (state != NULL){
        // set to null
        setStatemin();
    }
    char rank = p->getRank();
    state = p; // change
    // updates the pointers
    if (rank == 'k'){
        game->bk = this;
    } else if (rank == 'K'){
        game->wk = this;
    } else if (rank == 'b'){
        // iterate through the vector and look for the vector whose piece you moved
        for (vector <Cell *>::iterator i = game->bbishops.begin(); i != game->bbishops.end(); i++){
            // if the cell's piece pointer is the same as the piece being passed,
            // change the cell to this cell
            if ((*i)->getState() == p){
                *i = this;
            }
        }
    } else if (rank == 'B'){
        // iterate through the vector and look for the vector whose piece you moved
        for (vector <Cell *>::iterator i = game->wbishops.begin(); i != game->wbishops.end(); i++){
            // if the cell's piece pointer is the same as the piece being passed,
            // change the cell to this cell
            if ((*i)->getState() == p){
                *i = this;
            }
        }
    } else if (rank == 'n'){
        // iterate through the vector and look for the vector whose piece you moved
        for (vector <Cell *>::iterator i = game->bknights.begin(); i != game->bknights.end(); i++){
            // if the cell's piece pointer is the same as the piece being passed,
            // change the cell to this cell
            if ((*i)->getState() == p){
                *i = this;
            }
        }
    } else if (rank == 'N'){
        // iterate through the vector and look for the vector whose piece you moved
        for (vector <Cell *>::iterator i = game->wknights.begin(); i != game->wknights.end(); i++){
            // if the cell's piece pointer is the same as the piece being passed,
            // change the cell to this cell
            if ((*i)->getState() == p){
                *i = this;
            }
        }
    } else if (rank == 'r'){
        // iterate through the vector and look for the vector whose piece you moved
        for (vector <Cell *>::iterator i = game->brooks.begin(); i != game->brooks.end(); i++){
            // if the cell's piece pointer is the same as the piece being passed,
            // change the cell to this cell
            if ((*i)->getState() == p){
                *i = this;
            }
        }
    } else if (rank == 'R'){
        // iterate through the vector and look for the vector whose piece you moved
        for (vector <Cell *>::iterator i = game->wrooks.begin(); i != game->wrooks.end(); i++){
            // if the cell's piece pointer is the same as the piece being passed,
            // change the cell to this cell
            if ((*i)->getState() == p){
                *i = this;
            }
        }
    } else if (rank == 'p'){
        // iterate through the vector and look for the vector whose piece you moved
        for (vector <Cell *>::iterator i = game->bpawns.begin(); i != game->bpawns.end(); i++){
            // if the cell's piece pointer is the same as the piece being passed,
            // change the cell to this cell
            if ((*i)->getState() == p){
                *i = this;
            }
        }
    } else if (rank == 'P'){
        // iterate through the vector and look for the vector whose piece you moved
        for (vector <Cell *>::iterator i = game->wpawns.begin(); i != game->wpawns.end(); i++){
            // if the cell's piece pointer is the same as the piece being passed,
            // change the cell to this cell
            if ((*i)->getState() == p){
                *i = this;
            }
        }
    } else if (rank == 'q'){
        // iterate through the vector and look for the vector whose piece you moved
        for (vector <Cell *>::iterator i = game->bqueens.begin(); i != game->bqueens.end(); i++){
            // if the cell's piece pointer is the same as the piece being passed,
            // change the cell to this cell
            if ((*i)->getState() == p){
                *i = this;
            }
        }
    } else if (rank == 'Q'){
        // iterate through the vector and look for the vector whose piece you moved
        for (vector <Cell *>::iterator i = game->wqueens.begin(); i != game->wqueens.end(); i++){
            // if the cell's piece pointer is the same as the piece being passed,
            // change the cell to this cell
            if ((*i)->getState() == p){
                *i = this;
            }
        }
    }
    notifyGame();
}

/*****setStatemin****
 * deletes the piece off the cell
 *********************/
void Cell::setStatemin(){ // mofidy
    if (!state){
        // there's no piece on the cell; do nothing
        return;
    }
    char rank = state->getRank();
    // updates the pointer to pieces in game as well
    if (rank == 'q'){
        for (vector <Cell *>::iterator i = game->bqueens.begin(); i != game->bqueens.end();){
            // if the grid's destination cell is the same cell that holds a black queen, erase it
            if (&game->theGrid[r][c]  == *i){
                i = game->bqueens.erase(i);
            } else {
                i++;
            }
        }
    }  else if (rank == 'Q'){
        for (vector <Cell *>::iterator i = game->wqueens.begin(); i != game->wqueens.end();){
            // if the grid's destination cell is the same cell that holds a white queen, erase it
            if (&game->theGrid[r][c]  == *i){
                i = game->wqueens.erase(i);
            } else {
                i++;
            }
        }
    } else if (rank == 'n'){
        for (vector <Cell *>::iterator i = game->bknights.begin(); i != game->bknights.end();){
            // if the grid's destination cell is the same cell that holds a black knight, erase it
            if (&game->theGrid[r][c]  == *i){
                i = game->bknights.erase(i);
            } else {
                i++;
            }
        }
    } else if (rank == 'N'){
        for (vector <Cell *>::iterator i = game->wknights.begin(); i != game->wknights.end();){
            // if the grid's destination cell is the same cell that holds a white knight, erase it
            if (&game->theGrid[r][c]  == *i){
                i = game->wknights.erase(i);
            } else {
                i++;
            }
        }
    } else if (rank == 'b'){
        for (vector <Cell *>::iterator i = game->bbishops.begin(); i != game->bbishops.end();){
            // if the grid's destination cell is the same cell that holds a black bishop, erase it
            if (&game->theGrid[r][c]  == *i){
                i = game->bbishops.erase(i);
            } else {
                i++;
            }
        }
    } else if (rank == 'B'){
        for (vector <Cell *>::iterator i = game->wbishops.begin(); i != game->wbishops.end();){
            // if the grid's destination cell is the same cell that holds a white bishop, erase it
            if (&game->theGrid[r][c]  == *i){
                i = game->wbishops.erase(i);
            } else {
                i++;
            }
        }
    } else if (rank == 'r'){
        for (vector <Cell *>::iterator i = game->brooks.begin(); i != game->brooks.end();){
            // if the grid's destination cell is the same cell that holds a black rook, erase it
            if (&game->theGrid[r][c]  == *i){
                i = game->brooks.erase(i);
            } else {
                i++;
            }
        }
    } else if (rank == 'R'){
        for (vector <Cell *>::iterator i = game->wrooks.begin(); i != game->wrooks.end();){
            // if the grid's destination cell is the same cell that holds a white rook, erase it
            if (&game->theGrid[r][c]  == *i){
                i = game->wrooks.erase(i);
            } else {
                i++;
            }
        }
    } else if (rank == 'p'){
        for (vector <Cell *>::iterator i = game->bpawns.begin(); i != game->bpawns.end();){
            // if the grid's destination cell is the same cell that holds a black pawn, erase it
            if (&game->theGrid[r][c]  == *i){
                i = game->bpawns.erase(i);
            } else {
                i++;
            }
        }
    } else if (rank == 'P'){
        for (vector <Cell *>::iterator i = game->wpawns.begin(); i != game->wpawns.end();){
            // if the grid's destination cell is the same cell that holds a white pawn, erase it
            if (&game->theGrid[r][c]  == *i){
                i = game->wpawns.erase(i);
            } else {
                i++;
            }
        }
    }

    delete state;
    state = NULL;
    clearEats();
    notifyGame();
}

/*****setCoords*****
 * sets coordinates of the cell
 ********************/
void Cell::setCoords(const int r, const int c){
    this->r = r;
    this->c = c;
}

/*******selfpush*****
 * updates the pointer for the
 * rank you are moving
 *********************/
void Cell::selfpush(char rank){
    if (rank == 'k') {
        if (game->bk != NULL){ // assure one king
            game->bk->setStatemin(); // take out current king if a new king's been placed
        }
        game->bk = this;
    } else if (rank == 'K'){
        if (game->wk != NULL){ // assure one king
            game->wk->setStatemin(); // take out current king if a new king's been placed
        }
        game->wk = this;
    } else if (rank == 'q'){
        game->bqueens.push_back(this);
    } else if (rank == 'Q'){
        game->wqueens.push_back(this);
    } else if (rank == 'b'){
        game->bbishops.push_back(this);
    } else if (rank == 'B'){
        game->wbishops.push_back(this);
    } else if (rank == 'r'){
        game->brooks.push_back(this);
    } else if (rank == 'R'){
        game->wrooks.push_back(this);
    } else if (rank == 'n'){
        game->bknights.push_back(this);
    } else if (rank == 'N'){
        game->wknights.push_back(this);
    } else if (rank == 'p'){
        game->bpawns.push_back(this);
    } else if (rank == 'P'){
        game->wpawns.push_back(this);
    }
}

/******setGame*****
 * sets the game for cell
 *******************/
void Cell::setGame(Game *game){
    this->game = game;
}

/*****notifyGame****
 * notifies the game of the change in
 * the cell
 ********************/
void Cell::notifyGame(){
    game->notify(r,c);
}

/*******addRook*****
 * adds all the rook on the cell's possible
 * locations to go to the cell's possibleEats
 ********************/
void Cell::addRook(){
    char colour = getColour();
    int r = getR();
    int c = getC();

    //up check ~ same col
    int rup = r-1;
    while(withinBoard(rup,c)){
        Cell* pos = &game->theGrid[rup][c];
        if (pos->getState() == NULL){
            possibleEats.push_back(pos);
        } else {
            if (pos->getColour() != colour){
                possibleEats.push_back(pos);
            }
            break;
        }
        rup--;
    }
    //down check ~ same col
    int rdown = r+1;
    while(withinBoard(rdown,c)){
        Cell *pos = &game->theGrid[rdown][c];

        if (pos->getState() == NULL){
            possibleEats.push_back(pos);
        } else {
            if (pos->getColour() != colour){
                possibleEats.push_back(pos);
            }
            break;
        }
        rdown++;
    }
    //left check ~ same row
    int cleft = c-1;
    while(withinBoard(r,cleft)){
        Cell *pos = &game->theGrid[r][cleft];
        if (pos->getState() == NULL){
            possibleEats.push_back(pos);
        } else {
            if (pos->getColour() != colour){
                possibleEats.push_back(pos);
            }
            break;
        }
        cleft--;
    }
    //right check ~ same row
    int cright = c+1;
    while(withinBoard(r,cright)){
        Cell *pos = &game->theGrid[r][cright];
        if (pos->getState() == NULL){
            possibleEats.push_back(pos);
        } else {
            if (pos->getColour() != colour){
                possibleEats.push_back(pos);
            }
            break;
        }
        cright++;
    }
}

/*******addBishop*******
 * adds the possible locations that a bishop
 * could eat at the cell's location to the cell's
 * possibleEats
 ************************/
void Cell::addBishop(){
    char colour = getColour();
    int r = getR();
    int c = getC();

    // up left diagonal add
    int mr = r-1;
    int mc = c-1;
    while (withinBoard(mr,mc)){
        Cell *pos = &game->theGrid[mr][mc];
        if (pos->getState() == NULL ){
            possibleEats.push_back(pos);
        } else {
            if (pos->getColour() != colour){
                possibleEats.push_back(pos);
            }
            break;
        }
        mr--;
        mc--;
    }
    // up right diagonal add
    mr = r-1;
    mc = c+1;
    while (withinBoard(mr,mc)){
        Cell *pos = &game->theGrid[mr][mc];
        if (pos->getState() == NULL ){
            possibleEats.push_back(pos);
        } else {
            if (pos->getColour() != colour){
                possibleEats.push_back(pos);
            }
            break;
        }
        mr--;
        mc++;
    }
    // down right diagonal add
    mr = r+1;
    mc = c+1;
    while (withinBoard(mr,mc)){
        Cell *pos = &game->theGrid[mr][mc];
        if (pos->getState() == NULL ){
            possibleEats.push_back(pos);
        } else {
            if (pos->getColour() != colour){
                possibleEats.push_back(pos);
            }
            break;
        }
        mr++;
        mc++;
    }
    // down left diagonal add
    mr = r+1;
    mc = c-1;
    while (withinBoard(mr,mc)){
        Cell *pos = &game->theGrid[mr][mc];
        if (pos->getState() == NULL ){
            possibleEats.push_back(pos);
        } else {
            if (pos->getColour() != colour){
                possibleEats.push_back(pos);
            }
            break;
        }
        mr++;
        mc--;
    }


}

/***addToEats***
 * adds a cell to the cell's possible eating locations
 ****************/
void Cell::addToEats(){
    char rank = getRank();
    char colour = getColour();
    possibleEats.clear();
    //pawn add
    //pos means possible
    if (rank == 'P'){
        if (withinBoard(r-1,c-1)){
            Cell *pos = &game->theGrid[r-1][c-1];
            if (pos->getColour() != colour && pos->state != NULL) {
              possibleEats.push_back(pos);
            }
            Cell *pawn = &game->theGrid[r][c-1];
            if (getEnpass() && pawn->getColour() != colour && pawn->state != NULL) {
              possibleEats.push_back(pos);
            }
        }
        if (withinBoard(r-1,c+1)){
            Cell *pos = &game->theGrid[r-1][c+1];
            if (pos->getColour() != colour && pos->state != NULL) {
              possibleEats.push_back(pos);
            }
            Cell *pawn = &game->theGrid[r][c+1];
            if (getEnpass() && pawn->getColour() != colour && pawn->state != NULL){
              possibleEats.push_back(pos);
            }
        }
    }
    else if (rank == 'p'){
        if (withinBoard(r+1,c-1)){
            Cell *pos = &game->theGrid[r+1][c-1];
            if ((pos->getColour() != colour) && pos->state != NULL) {
              possibleEats.push_back(pos);
            }
            Cell *pawn = &game->theGrid[r][c-1];
            if (getEnpass() && pawn->getColour() != colour && pawn->state != NULL){
              possibleEats.push_back(pos);
            }
        }
        if (withinBoard(r+1,c+1)){
            Cell *pos = &game->theGrid[r+1][c+1];
            if (pos->getColour() != colour && pos->state != NULL){
              possibleEats.push_back(pos);
            }
            Cell *pawn = &game->theGrid[r][c+1];
            if (getEnpass() && pawn->getColour() != colour && pawn->state != NULL) {
              possibleEats.push_back(pos);
            }
        }
    }

    //knight add
    else if (rank == 'N' || rank == 'n'){
        if (withinBoard(r-1,c-2)){
            Cell *pos = &game->theGrid[r-1][c-2];
            if (pos->getColour() != colour) possibleEats.push_back(pos);
        }
        if (withinBoard(r-2,c-1)){
            Cell *pos = &game->theGrid[r-2][c-1];
            if (pos->getColour() != colour) possibleEats.push_back(pos);
        }
        if (withinBoard(r-2,c+1)){
            Cell *pos = &game->theGrid[r-2][c+1];
            if (pos->getColour() != colour) possibleEats.push_back(pos);
        }
        if (withinBoard(r-1,c+2)){
            Cell *pos = &game->theGrid[r-1][c+2];
            if (pos->getColour() != colour) possibleEats.push_back(pos);
        }
        if (withinBoard(r+1,c+2)){
            Cell *pos = &game->theGrid[r+1][c+2];
            if (pos->getColour() != colour) possibleEats.push_back(pos);
        }
        if (withinBoard(r+2,c+1)){
            Cell *pos = &game->theGrid[r+2][c+1];
            if (pos->getColour() != colour) possibleEats.push_back(pos);
        }
        if (withinBoard(r+2,c-1)){
            Cell *pos = &game->theGrid[r+2][c-1];
            if (pos->getColour() != colour) possibleEats.push_back(pos);
        }
        if (withinBoard(r+1,c-2)){
            Cell *pos = &game->theGrid[r+1][c-2];
            if (pos->getColour() != colour) possibleEats.push_back(pos);
        }
    }

    //rook add
    else if (rank == 'R' || rank == 'r'){
        addRook();
    }

    else if (rank == 'B' || rank == 'b'){
        addBishop();
    }

    else if (rank == 'Q' || rank == 'q'){
        addRook();
        addBishop();
    }
    else if (rank == 'K' || rank == 'k'){
        if (withinBoard(r-1,c-1)){
            Cell *pos = &game->theGrid[r-1][c-1];
            if (pos->getColour() != colour) possibleEats.push_back(pos);
        }
        if (withinBoard(r-1,c)){
            Cell *pos = &game->theGrid[r-1][c];
            if (pos->getColour() != colour) possibleEats.push_back(pos);
        }
        if (withinBoard(r-1,c+1)){
            Cell *pos = &game->theGrid[r-1][c+1];
            if (pos->getColour() != colour) possibleEats.push_back(pos);
        }
        if (withinBoard(r+1,c+1)){
            Cell *pos = &game->theGrid[r+1][c+1];
            if (pos->getColour() != colour) possibleEats.push_back(pos);
        }
        if (withinBoard(r,c-1)){
            Cell *pos = &game->theGrid[r][c-1];
            if (pos->getColour() != colour) possibleEats.push_back(pos);
        }
        if (withinBoard(r,c+1)){
            Cell *pos = &game->theGrid[r][c+1];
            if (pos->getColour() != colour) possibleEats.push_back(pos);
        }
        if (withinBoard(r+1,c-1)){
            Cell *pos = &game->theGrid[r+1][c-1];
            if (pos->getColour() != colour) possibleEats.push_back(pos);
        }
        if (withinBoard(r+1,c)){
            Cell *pos = &game->theGrid[r+1][c];
            if (pos->getColour() != colour) possibleEats.push_back(pos);
        }
    }
}

/***getEats****
 * returns the vector of possible eating locations by cell
 ***************/
std::vector<Cell *> Cell::getEats(){
    return possibleEats;
}

/*****addAdd****
 * adds c to the cell's
 * possibleEats
 ****************/
void Cell::addAdd(Cell* c){
    possibleEats.push_back(c);
}

/******eraseErase***
 * removes the cell at index
 * from the cell's possibleEats
 *********************/
void Cell::eraseErase(int index){
    possibleEats.erase(possibleEats.begin()+index);
}

/*****stillInCheck****
 * returns true if a king is still
 * in check after moving the cell's
 * piece to new_r, new_c. False otherwise
 **********************/
bool Cell::stillInCheck(int new_r, int new_c){
    //temporarily change the state of the grid; doesn't change ptrs, etc
    Piece *temp = state;
    Piece *oldCellState = game->theGrid[new_r][new_c].getState();
    game->theGrid[new_r][new_c].tempSetState(temp);
    game->theGrid[r][c].tempSetState(NULL);

    // first get the rank of the king using the game turn
    // then get the location of that king using the pointer if the piece
    // that was moved was not a king
    char turn = game->getTurn();
    char kingRank;
    int kingR;
    int kingC;
    bool inCheck;
    if (turn == 'b'){
        kingRank = 'k';
        kingR = game->bk->getR();
        kingC = game->bk->getC();
    } else if (turn == 'w'){
        kingRank = 'K';
        kingR = game->wk->getR();
        kingC = game->wk->getC();
    } else {
        kingRank = 'K';
        kingR = game->wk->getR();
        kingC = game->wk->getC();
    }
    // if the piece that was moved was the king, change the kingR and
    // kingC to be the temporarily moved location
    if (temp->getRank() == kingRank){
        kingR = new_r;
        kingC = new_c;
    }

    //location of king in check
    // if the king is still in check, then invalid move; return true
    if (game->canBeEaten(turn, kingR, kingC,false)){
        inCheck = true;
    } else {
        inCheck = false;
    }
    // change it back
    game->theGrid[new_r][new_c].tempSetState(oldCellState);
    game->theGrid[r][c].tempSetState(temp);

    return inCheck;
}
