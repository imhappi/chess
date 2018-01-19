#include "graphicdisplay.h"
#include <string>
#include <sstream>

int gridSize = 8;
int white = 0;
int myBoardSize = 550;
int windowSizey = 650;
int windowSizex = windowSizey - 40;
int black = 1;
int red = 2;
int blue = 4;
int cyan = 5;
int orange = 8;
int brown = 9;

using namespace std;

/****GraphicDisplay***
 * GraphicDisplay constructor
 **********************/
GraphicDisplay::GraphicDisplay():View(){
    window = new Xwindow(windowSizex,windowSizey);
    window->fillRectangle(0,0,windowSizex,windowSizey, white);
    int rCoord = gridSize;
    for (int r = 0; r < gridSize; r++){
        stringstream ss;
        ss << rCoord;
        string str = ss.str();
        window->drawBigString(30,(myBoardSize/gridSize) * (r+1) - 20,str,black);
        for (int c = 0; c < gridSize; c++){
            if (((r % 2 == 1) && (c % 2 == 0)) || ((r % 2 == 0) && (c % 2 == 1))){
                window->fillRectangle(myBoardSize/gridSize * (c + 1), myBoardSize/gridSize * (7 - r),
                        myBoardSize/gridSize, myBoardSize/gridSize, orange);
            } else {
                window->fillRectangle(myBoardSize/gridSize * (c + 1), myBoardSize/gridSize * (7 - r),
                        myBoardSize/gridSize, myBoardSize/gridSize, brown);
            }
        }
        rCoord--;
    }

    char letter = 'a';
    for (int c = 0; c < gridSize; c++){
        stringstream ss;
        ss << letter;
        string str = ss.str();
        window->drawBigString(myBoardSize/gridSize * (c+1) + 20, myBoardSize/gridSize + myBoardSize - 30, str,black);
        letter++;
    }
}

/****~GraphicDisplay****
 * GraphicDisplay destructor
 ************************/
GraphicDisplay::~GraphicDisplay(){
    delete window;
}

/****notify****
 * controler calls notify to
 * update the (r,c) location in the display to be changed
 * to empty, and prints the new piece in new_r, new_c
 ***************/
void GraphicDisplay::notify(int r, int c, char piece){
    if (piece == ' '){
        window->fillRectangle(myBoardSize/gridSize * (c + 1), myBoardSize/gridSize * (7 - r),
                myBoardSize/gridSize, myBoardSize/gridSize, orange);
        return;
    } else if (piece == '_'){
        window->fillRectangle(myBoardSize/gridSize * (c + 1), myBoardSize/gridSize * (7 - r),
                myBoardSize/gridSize, myBoardSize/gridSize, brown);
        return;
    }

    if (piece == 'k'){
        window->drawBigString(myBoardSize/gridSize * (c + 1) + 20, myBoardSize/gridSize * (8 - r) - 20,"k",black);
    } else if (piece == 'q'){
        window->drawBigString(myBoardSize/gridSize * (c + 1) + 20, myBoardSize/gridSize * (8 - r) - 20,"q",black);
    } else if (piece == 'r'){
        window->drawBigString(myBoardSize/gridSize * (c + 1) + 20, myBoardSize/gridSize * (8 - r) - 20,"r",black);
    } else if (piece == 'b'){
        window->drawBigString(myBoardSize/gridSize * (c + 1) + 20, myBoardSize/gridSize * (8 - r) - 20,"b",black);
    } else if (piece == 'n'){
        window->drawBigString(myBoardSize/gridSize * (c + 1) + 20, myBoardSize/gridSize * (8 - r) - 20,"n",black);
    } else if (piece == 'p'){
        window->drawBigString(myBoardSize/gridSize * (c + 1) + 20, myBoardSize/gridSize * (8 - r) - 20,"p",black);
    } else if (piece == 'K'){
        window->drawBigString(myBoardSize/gridSize * (c + 1) + 20, myBoardSize/gridSize * (8 - r) - 20,"K",white);
    } else if (piece == 'Q'){
        window->drawBigString(myBoardSize/gridSize * (c + 1) + 20, myBoardSize/gridSize * (8 - r) - 20,"Q",white);
    } else if (piece == 'R'){
        window->drawBigString(myBoardSize/gridSize * (c + 1) + 20, myBoardSize/gridSize * (8 - r) - 20,"R",white);
    } else if (piece == 'B'){
        window->drawBigString(myBoardSize/gridSize * (c + 1) + 20, myBoardSize/gridSize * (8 - r) - 20,"B",white);
    } else if (piece == 'N'){
        window->drawBigString(myBoardSize/gridSize * (c + 1) + 20, myBoardSize/gridSize * (8 - r) - 20,"N",white);
    } else if (piece == 'P'){
        window->drawBigString(myBoardSize/gridSize * (c + 1) + 20, myBoardSize/gridSize * (8 - r) - 20,"P",white);
    }
}

void GraphicDisplay::print(std::ostream &out){}
