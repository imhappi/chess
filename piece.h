#ifndef __PIECE_H__
#define __PIECE_H__

class Piece{

    protected:
        char colour; //b for black, w for white
        char rank;
        bool first;
        bool enpassant;
    public:
        virtual bool eat(int old_r, int old_c, int new_r, int new_c) = 0;
        virtual bool move(int old_r, int old_c, int new_r, int new_c) = 0;
        char getColour();
        char getRank();
        bool getFirst();
        void setFirst(bool first);
        void setEnpass(bool enpass);
        bool getEnpass();
        virtual ~Piece();
};
#endif
