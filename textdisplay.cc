#include "textdisplay.h"
#include <iostream>

// textdisplay ctor
TextDisplay::TextDisplay():View(){
    theDisplay = new char*[gridSize];
    for(int i = 0; i < gridSize; i++){
        theDisplay[i] = new char[gridSize];
        for (int k = 0; k < gridSize; k++){
            if (i % 2 == 0 && k % 2 == 0){
                theDisplay[i][k] = ' ';
            }else if (i % 2 != 0 && k % 2 != 0){
                theDisplay[i][k] = ' ';
            }else  {
                theDisplay[i][k] = '_';
            }
        }
    }
}
// textdisplay dtor
TextDisplay::~TextDisplay(){
    for (int i = 0; i < gridSize; i++){
        delete[] theDisplay[i];
    }
    delete[] theDisplay;
}

// updates the textdisplay
void TextDisplay::notify(int r, int c, char ch){
    theDisplay[7-r][c] = ch;
}
// prints the textdisplay
void TextDisplay::print(std::ostream &out){
    int col = 8;
    for (int i = 0; i < gridSize; i++){
        out << col << " ";
        col--;
        for (int k = 0; k < gridSize; k++){
            out << theDisplay[i][k];
        }
        out << std::endl;
    }
    out << std::endl;
    out << "  abcdefgh" << std::endl;
}
