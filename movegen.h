#ifndef MOVEGEN_H
#define MOVEGEN_H
#include <vector>
bool islegalmove(int fx, int fy, int tx, int ty);
bool ischeck(int side);
struct Move
{
      int fx, fy, tx, ty;
      char cpiece;  
};
std::vector<Move> generatemoves(int side);
void makemove(Move m);
void undomove(Move m);
int evaluate();
int minimax(int depth, bool white); 
Move findbestmove(bool white,int depth);
#endif