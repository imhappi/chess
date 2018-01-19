#include "controller.h"
#include <iostream>
#include <string>

using namespace std;

int main (int argc, char *argv[]) {
    float ws = 0;
    float bs = 0;
    int won;
    if (argc == 2){ // implementing saved game.
        string arg = argv[1];
        Controller c;
        won = c.play(true,arg);
        if (won == 'w'){
            ws++;
        } else  if (won == 'b'){
            bs++;
        } else if (won == 't'){
            ws += 0.5;
            bs += 0.5;
        }
    }

    while (true){
        Controller c;
        won = c.play(false);
        if (cin.eof()) break;
        if (won == 'w'){
            ws++;
        } else if (won == 'b'){
            bs++;
        } else {
            ws += 0.5;
            bs += 0.5;
        }
    }
    cout << "Final Score:" << endl;
    cout << "White: " << ws << endl;
    cout << "Black: " << bs << endl;
}
