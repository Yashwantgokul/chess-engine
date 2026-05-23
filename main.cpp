#include <iostream>
#include <cctype>
#include "board.h"
#include "movegen.h"
using namespace std;
int main() {
    initializeBoard();
    bool whiteturn = true;
    while (true) {
        
        printBoard();
        string move;
        if(whiteturn) {
            cout << "White's turn." << endl;
            cout << "Enter your move (e.g., e2e4): ";
            cin >> move;
            if (move == "exit") {
                break;
            }
            if(move.length()!=4){
                cout<<"Invalid format!"<<endl;
                continue;
            }
            int fy = move[0] - 'a';
            int fx = 8 - (move[1] - '0');

            int ty = move[2] - 'a';
            int tx = 8 - (move[3] - '0');
            char piece = board[fx][fy];
            if(islower(piece)){
                cout<<"It's Black piece!"<<endl;
                continue;
            }
            if(!islegalmove(fx, fy, tx, ty)){
                cout<<"Illegal move!"<<endl;
                continue;
            }
            Move m;
            m.fx=fx;
            m.fy=fy;
            m.tx=tx;
            m.ty=ty;
            m.cpiece=board[tx][ty];
            makemove(m);
            whiteturn=!whiteturn;
        }
        else{
            cout << "Computer's turn." << endl;
            Move bestmove = findbestmove(false, 3);
            if(bestmove.fx==-1){
                if(ischeck(0)){
                    cout<<"Checkmate! White wins!"<<endl;
                }
                else{
                    cout<<"Stalemate!"<<endl;
                }
                break;
            }
            makemove(bestmove);
            cout << "Computer moves: " << char(bestmove.fy + 'a') << (8 - bestmove.fx) << char(bestmove.ty + 'a') << (8 - bestmove.tx) << endl;
            whiteturn=!whiteturn;

        }
        
    }
    return 0;
}