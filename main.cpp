#include <iostream>
#include <cctype>
#include "board.h"
#include "movegen.h"
using namespace std;
int main() {
    initializeBoard();
    int whiteturn = 1;
    while (true) {
        
        printBoard();
        string move;
        if(whiteturn) {
            cout << "White's turn." << endl;
        } else {            cout << "Black's turn." << endl;
        }
        
        cout << "Enter your move (e.g., e2e4): ";
        cin >> move;
        if (move == "exit") {
            break;
        }
        int fy = move[0] - 'a';
        int fx = 8 - (move[1] - '0');

        int ty = move[2] - 'a';
        int tx = 8 - (move[3] - '0');
        char piece = board[fx][fy];
        if(whiteturn && islower(piece)){
            cout<<"It's White's turn!"<<endl;
            continue;
        }

        if(!whiteturn && isupper(piece)){
            cout<<"It's Black's turn!"<<endl;
            continue;
        }
        if(piece == ' ' || (isupper(piece) && isupper(board[tx][ty])) || (islower(piece) && islower(board[tx][ty]))||!islegalmove(fx, fy, tx, ty)) {
            cout << "Invalid move. Try again." << endl;
            continue;
        }
        board[tx][ty] = board[fx][fy];
        board[fx][fy] = '.';
        whiteturn = !whiteturn;
    }
    return 0;
}