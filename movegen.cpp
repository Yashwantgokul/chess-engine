#include "movegen.h"
#include "board.h"
#include <cmath>
#include <cctype>

using namespace std;
bool whitepawnmove(int fx, int fy, int tx, int ty);
bool blackpawnmove(int fx, int fy, int tx, int ty);
bool bishopmove(int fx, int fy, int tx, int ty);
bool rookmove(int fx, int fy, int tx, int ty);
bool knightmove(int fx, int fy, int tx, int ty);
bool queenmove(int fx, int fy, int tx, int ty); 
bool kingmove(int fx, int fy, int tx, int ty);
bool islegalmove(int fx, int fy, int tx, int ty) {
    char piece = board[fx][fy];
    char target = board[tx][ty];
    if (piece == ' ' || (isupper(piece) && isupper(target)) || (islower(piece) && islower(target))) {
        return false;
    }
    if(piece=='P'){
        return whitepawnmove(fx, fy, tx, ty);
    }
    if(piece=='p'){
        return blackpawnmove(fx, fy, tx, ty);
    }
    if(piece=='B'||piece=='b'){
        return bishopmove(fx, fy, tx, ty);
    }
    if(piece=='R'||piece=='r'){
        return rookmove(fx, fy, tx, ty);
    }
    if(piece=='N'||piece=='n'){
        return knightmove(fx, fy, tx, ty);
    }
    if(piece=='Q'||piece=='q'){
        return queenmove(fx, fy, tx, ty);
    }
    if(piece=='K'||piece=='k'){
        return kingmove(fx, fy, tx, ty);
    }
    // Add specific move rules for each piece type here
    return true; // Placeholder for legal move checking
    
}
bool whitepawnmove(int fx, int fy, int tx, int ty) {
    if (fx == 6 && tx == 4 && fy == ty && board[5][fy] == '.' && board[4][fy] == '.') {
        return true; // Initial two-square move
    }
    if (tx == fx - 1 && fy == ty && board[tx][ty] == '.') {
        return true; // Normal one-square move
    }
    if (tx == fx - 1 && abs(fy - ty) == 1 && islower(board[tx][ty])) {
        return true; // Capture move
    }
    return false;
}
bool blackpawnmove(int fx, int fy, int tx, int ty) {
    if (fx == 1 && tx == 3 && fy == ty && board[2][fy] == '.' && board[3][fy] == '.') {
        return true; // Initial two-square move
    }
    if (tx == fx + 1 && fy == ty && board[tx][ty] == '.') {
        return true; // Normal one-square move
    }
    if (tx == fx + 1 && abs(fy - ty) == 1 && isupper(board[tx][ty])) {
        return true; // Capture move
    }
    return false;
}
bool bishopmove(int fx, int fy, int tx, int ty) {
    if (abs(fx - tx) == abs(fy - ty)) {
        int xdir = (tx > fx) ? 1 : -1;
        int ydir = (ty > fy) ? 1 : -1;
        for (int i = 1; i < abs(fx - tx); i++) {
            if (board[fx + i * xdir][fy + i * ydir] != '.') {
                return false; // Path is blocked
            }
        }
        return true;
    }
    return false;
}
bool rookmove(int fx, int fy, int tx, int ty) {
    if (fx == tx) {
        int ydir = (ty > fy) ? 1 : -1;
        for (int i = 1; i < abs(fy - ty); i++) {
            if (board[fx][fy + i * ydir] != '.') {
                return false; // Path is blocked
            }
        }
        return true;
    }
    if (fy == ty) {
        int xdir = (tx > fx) ? 1 : -1;
        for (int i = 1; i < abs(fx - tx); i++) {
            if (board[fx + i * xdir][fy] != '.') {
                return false; // Path is blocked
            }
        }
        return true;
    }
    return false;
}
bool knightmove(int fx, int fy, int tx, int ty) {
    return (abs(fx - tx) == 2 && abs(fy - ty) == 1) || (abs(fx - tx) == 1 && abs(fy - ty) == 2);
}
bool queenmove(int fx, int fy, int tx, int ty) {
    return bishopmove(fx, fy, tx, ty) || rookmove(fx, fy, tx, ty);
}
bool kingmove(int fx, int fy, int tx, int ty) {
    return abs(fx - tx) <= 1 && abs(fy - ty) <= 1;
}