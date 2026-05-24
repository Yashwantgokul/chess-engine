#ifndef BOARD_H
#define BOARD_H

extern char board[8][8];

void initializeBoard();

void printBoard();
extern int whitekingrow, whitekingcol;
extern int blackkingrow, blackkingcol;
extern bool whitekingmoved, blackkingmoved;
extern bool whiteleftrookmoved, whiterightrookmoved;
extern bool blackleftrookmoved, blackrightrookmoved;
#endif