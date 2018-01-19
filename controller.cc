#include "controller.h"
#include "game.h"
#include "textdisplay.h"
#include "graphicdisplay.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <vector>

using namespace std;

// contoller ctor
Controller::Controller(): setup(false){
    game = new Game();
    td = NULL;
    white = NULL;
    black = NULL;
    gd = NULL;
}

/*****notify**
 * Update the View(s) to indicate the new game state
 *************/
void Controller::notify(int r, int c, char rank){
    td->notify(r,c,rank);
    gd->notify(r,c,rank);
}

/******init*******
 * Called to initialize setup, etc
 *****************/
void Controller::init(istream & input, Game & g , bool saved){
    std::string cmd;
    int r;
    char c;
    while (!saved){
        input >> cmd;
        if (input.eof()) break;

        if (cmd == "done") {
            //check if both king pointers are not NULL;
            if (game->bk && game->wk && !game->hasPawn() &&
                    !game->canBeEaten('b',game->bk->getR(),game->bk->getC()) &&
                    !game->canBeEaten('w',game->wk->getR(),game->wk->getC())){
                cout << "Successful setup" << endl;
                break;
            }
            if (game->hasPawn()){
                cout << "Invalid: there is a pawn in the first/last row" << endl;
            }
            if (game->bk && game->wk){
                if (game->canBeEaten('b',game->bk->getR(),game->bk->getC())){
                    cout << "Invalid: the black king is in check" << endl;
                }
                if (game->canBeEaten('w',game->wk->getR(),game->wk->getC())){
                    cout << "Invalid: the white king is in check" << endl;
                }
            } else {
                cout << "Invalid: Must be 2 kings on the board." << endl;
            }
        }
        // if you want to add a piece to the board
        if (cmd == "+"){
            char piece;
            input >> piece >> c >> r;
            game->init(r,c-'a',piece);
            td->print(cout);
        } else if (cmd == "-"){ // if you want to take away the piece
            input >> c >> r;
            game->takeOff(r,c-'a');
            td->print(cout);
        } else if (cmd == "="){ // if you want to set the turn
            std::string turn;
            input >> turn;
            if (turn == "white"){
                game->setTurn('w');
            } else if (turn == "black"){
                game->setTurn('b');
            } else {
                cout << "Valid turns are white and black. Try again" << endl;
                continue;
            }
        }
    }

    if (saved){
        int row = gridSize;
        char piece;
        while (row >= 0){
            for (int i = 0; i < gridSize ; i++){
                input >> piece;
                if (piece != '_') {
                    game->init(row,i,piece);
                }
            }
            row--;
        }
        input >> piece;
        if (piece == 'W'){
            game->setTurn('w');
        } else if (piece == 'B'){
            game->setTurn('b');
        }
    }
}

/*****savemode****
 * loads the save state file
 ******************/
void Controller::savemode(string saved){
    game->init(this);
    delete td;
    td = new TextDisplay();
    delete gd;
    gd = new GraphicDisplay();
    setup = true;
    ifstream input(saved.c_str());
    init(input, *game , true);
    td->print(cout);
}

/******gamemode******
 * initializes the game
 *********************/
void Controller::gamemode(){
    string user;
    //White player
    while (true){
        cin >> user;
        if (cin.eof()) return;
        if (user == "human"){
            white = Human::createWhite('w');
            break;
        } else if (user == "computer1") {
            white = Computer::createWhiteAi('w',1,game);
            break;
        } else if (user == "computer2") {
            white = Computer::createWhiteAi('w',2,game);
            break;
        } else if (user == "computer3") {
            white = Computer::createWhiteAi('w',3,game);
            break;
        } else if (user == "computer4") {
            white = Computer::createWhiteAi('w',4,game);
            break;
        } else {
            cout << "Not valid input! Options for players are human" <<
                ", computer1, computer2, computer3, computer4." << endl;
        }
    }
    //black player
    while (true){
        cin >> user;
        if (cin.eof()) return;
        if (user == "human"){
            black = Human::createBlack('b');
            break;
        } else if (user == "computer1") {
            black = Computer::createBlackAi('b',1,game);
            break;
        } else if (user == "computer2") {
            black = Computer::createBlackAi('b',2,game);
            break;
        } else if (user == "computer3") {
            black = Computer::createBlackAi('b',3,game);
            break;
        } else if (user == "computer4") {
            black = Computer::createBlackAi('b',4,game);
            break;
        } else {
            cout << "Not valid input! Options for players are human" <<
                ", computer1, computer2, computer3, computer4." << endl;
        }
    }

    if (!setup){
        //Grid initialization
        game->init(this);
        //Cleand Td
        delete td;
        //Td initialization
        td = new TextDisplay();

        delete gd;
        gd = new GraphicDisplay();

        //Hardcode Black
        //pawn
        for (char c = 'a'; c < 'i' ; c++){
            int r = 7;
            game->init(r,c-'a','p');
        }
        // code initial setup
        //rook
        game->init(8,0,'r');
        game->init(8,7,'r');
        //knight
        game->init(8,1,'n');
        game->init(8,6,'n');
        //bishop
        game->init(8,2,'b');
        game->init(8,5,'b');
        //king&Queen
        game->init(8,3,'q');
        game->init(8,4,'k');
        //Hardcode White
        //pawn
        for (char c = 'a'; c < 'i' ; c++){
            int r = 2;
            game->init(r,c-'a','P');
        }
        //rook
        game->init(1,0,'R');
        game->init(1,7,'R');
        //knight
        game->init(1,1,'N');
        game->init(1,6,'N');
        //bishop
        game->init(1,2,'B');
        game->init(1,5,'B');
        //king&Queen
        game->init(1,3,'Q');
        game->init(1,4,'K');

        game->setTurn('w');
        td->print(cout);
        setup = true;
    }
}

/******setupmode*****
 * initializes the setup
 *********************/
void Controller::setupmode(){
    if (!setup){
        game->init(this);
        delete td;
        td = new TextDisplay();

        delete gd;
        gd = new GraphicDisplay();

        setup = true;
    }
    td->print(cout);
    init(cin, *game);
}

/*****movemode****
 * gets the move and changes the state of the board
 * if it's a valid move
 ******************/
void Controller::movemode(){
    string move;
    if (game->getTurn() == 'w' && white->computer){\
        game->computerMove(static_cast<Computer *>(white));
        td->print(cout);
        return;
    }
    if (game->getTurn() == 'b' && black->computer){
        game->computerMove(static_cast<Computer *>(black));

        td->print(cout);
        return;
    } else if (game->getTurn() == 'w'){
        move = white->getMove();
    } else {
        move = black->getMove();
    }
    int old_r, new_r, old_c , new_c;
    old_c = move[0] - 'a';
    old_r = move[1] - '0' - 1;
    new_c = move[2] - 'a';
    new_r = move[3] - '0' - 1;
    //check if the there is a fifth parameter i.e promotion char.
    if (move[4] == '\0'){
        game->notify(7-old_r,old_c,7-new_r,new_c);
        td->print(cout);
    } else { //if there exist one then pass it.
        game->notify(7-old_r,old_c,7-new_r,new_c,move[4]);
        td->print(cout);
    }
}

/*******play******
 * actual skeleton of game
 ******************/
char Controller::play(bool save,string saved){
    string cmd;
    if (save){//saved game case.
        //board initialized to set saved game.
        savemode(saved);
        game->enpassReset(); // resets the enpass value pawns.
        //game->addPass();
        if (game->checkmateCheck()){ // check for checkmate.
            cout << "Checkmate! " << endl;
            //game->switchTurn();
            return finish(game->getTurn());
        } else if (game->stalemateCheck()|| game->allVectorsEmpty()){ // check for stalemate
            cout << "Stalemate! " << endl;
            return finish('t');
        }
        checkFinish();  // check for general check/
    }
    while (cin >> cmd) {
        if (cin.eof()) break;
        //Game, sets a standard board if there is none and sets players.

        if (cmd == "game" && !white && !black) gamemode();
        //Setup, manually setup a board.
        else if (cmd == "setup")setupmode();
        //Move, obtains input from computer or cin and feeds it to the program.
        else if (cmd == "move"){
            if (!white){
                cout << "You need to initialize the game first." << endl;
                continue;
            }
            if (!black){
                cout << "You need to initialize the game first." << endl;
                continue;
            }
            movemode();
            game->enpassReset(); // resets the enpass value pawns.
            if (game->checkmateCheck()){ // check for checkmate.
                cout << "Checkmate! ";
                return finish(game->getTurn());
            } else if (game->stalemateCheck() || game->allVectorsEmpty()){ // check for stalemate
                cout << "Stalemate!";
                return finish('t');
            }
            checkFinish();  // check for general check/
        }
        else if (cmd == "resign"){
            char losingPlayer = game->getTurn();
            setup = 0;
            return finish(losingPlayer);
        } else {
            cout << "Invalid command, try again." << endl;
        }
    }
    return 0;
}

/*******finish******
 * passes off the points to the main program
 ********************/
char Controller::finish(char losingPlayer){
    if (losingPlayer == 'w'){
        cout << "Black wins!" << endl;
        return 'b';
    } else if (losingPlayer == 'b'){
        cout << "White wins!" << endl;
        return 'w';
    } else {
        return 't';
    }
}

/******checkFinish*******
 * prints out if a player is in check
 *************************/
void Controller::checkFinish(){
    if (game->getTurn() == 'b' && game->isCheck){
        cout << "Black is in check." << endl;
    } else if (game->getTurn() == 'w' && game->isCheck){
        cout << "White is in check." << endl;
    }
}

// controller dtor
Controller::~Controller(){
    delete game;
    delete td;
    delete gd;
    delete white;
    delete black;
}
