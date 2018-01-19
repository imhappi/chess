#include "piece.h"

class Knight : public Piece{
    public:
        bool move(int old_r, int old_c, int new_r, int new_c);
        bool eat(int old_r, int old_c, int new_r, int new_c);
        Knight(char rank);
};

