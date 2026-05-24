#include <iostream>
#include "board.h"
using namespace std;

char board[8][8];
char tempBoard[8][8]={
    {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'},
    {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
    {'.', '.', '.', '.', '.', '.', '.', '.'},
    {'.', '.', '.', '.', '.', '.', '.', '.'},
    {'.', '.', '.', '.', '.', '.', '.', '.'},
    {'.', '.', '.', '.', '.', '.', '.', '.'},
    {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
    {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'}
};
int whitekingrow = 7, whitekingcol = 4;
int blackkingrow = 0, blackkingcol = 4;
bool whitekingmoved=false, blackkingmoved=false;
bool whiteleftrookmoved=false, whiterightrookmoved=false;
bool blackleftrookmoved=false, blackrightrookmoved=false;
void initializeBoard() {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            board[i][j] = tempBoard[i][j];
        }
    }
}
void printBoard() {
    cout << "  a b c d e f g h" << endl;
    for (int i = 0; i < 8; i++) {
        cout << 8 - i << " ";
        for (int j = 0; j < 8; j++) {
            cout << board[i][j] << " ";
        }
        cout << 8 - i << endl;
    }
    cout << "  a b c d e f g h" << endl;
}