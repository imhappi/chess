#include "piece.h"

class Bishop : public Piece{
    public:
        bool move(int old_r, int old_c, int new_r, int new_c);
        bool eat(int old_r, int old_c, int new_r, int new_c);
        Bishop(char rank);
};

