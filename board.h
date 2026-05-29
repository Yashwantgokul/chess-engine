#ifndef BOARD_H
#define BOARD_H

extern char board[8][8];

void initializeBoard();

void printBoard();
extern int whitekingrow, whitekingcol;
extern int blackkingrow, blackkingcol;
extern bool whitekingmoved, blackkingmoved;
extern bool whitequeenrookmoved, whitekingrookmoved;
extern bool blackqueenrookmoved, blackkingrookmoved;
extern int enpassantrow, enpassantcol;
extern int knightTable[8][8], pawnTable[8][8], bishopTable[8][8], kingTable[8][8], rookTable[8][8], queenTable[8][8];
#endif