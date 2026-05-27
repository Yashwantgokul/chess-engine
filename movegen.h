#ifndef MOVEGEN_H
#define MOVEGEN_H
#include <vector>
bool islegalmove(int fx, int fy, int tx, int ty);
bool ischeck(int side);
struct Move
{
    int fx, fy;
    int tx, ty;

    char cpiece;

    // special moves
    bool castling = false;
    bool promotion = false;
    bool enpassant = false;
    char promotedpiece = '.';
    char movedpiece = '.';

    // restore previous state

    bool prevkingmoved = false;

    bool prevkingrookmoved = false;
    bool prevqueenrookmoved = false;

    // en passant restoration
    int prevenpassantrow = -1;
    int prevenpassantcol = -1;
    int score=0;
};
std::vector<Move> generatemoves(int side);
void makemove(Move &m);
void undomove(Move &m);
int evaluate();
int minimax(int depth, bool white); 
Move findbestmove(bool white,int depth);
#endif