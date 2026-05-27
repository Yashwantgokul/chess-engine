#include "movegen.h"
#include "board.h"
#include <algorithm>
#include <cmath>
#include <cctype>
#include <vector>
using namespace std;
void makemove(Move &m);
void undomove(Move &m);
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
    bool valid = false;
    if(target == 'K' || target == 'k') {
        return false;
    }
    if (piece == '.' || (isupper(piece) && isupper(target)) || (islower(piece) && islower(target))) {
        return false;
    }
    if(piece=='P'){
        valid=whitepawnmove(fx, fy, tx, ty);
    }
    if(piece=='p'){
        valid=blackpawnmove(fx, fy, tx, ty);
    }
    if(piece=='B'||piece=='b'){
        valid=bishopmove(fx, fy, tx, ty);
    }
    if(piece=='R'||piece=='r'){
        valid=rookmove(fx, fy, tx, ty);
    }
    if(piece=='N'||piece=='n'){
        valid=knightmove(fx, fy, tx, ty);
    }
    if(piece=='Q'||piece=='q'){
        valid=queenmove(fx, fy, tx, ty);
    }
    if(piece=='K'||piece=='k'){
        valid=kingmove(fx, fy, tx, ty);
    }
    // Add specific move rules for each piece type here
    if(!valid) // Placeholder for legal move checking
        return false;
    bool enPassantCapture = false;
    int enPassantCaptureRow = -1;
    char enPassantCapturedPiece = '.';

    if(piece == 'P' && tx == enpassantrow && ty == enpassantcol && target == '.' && abs(fy - ty) == 1 && fx - 1 == tx && board[fx][ty] == 'p')
    {
        enPassantCapture = true;
        enPassantCaptureRow = fx;
        enPassantCapturedPiece = board[enPassantCaptureRow][ty];
        board[enPassantCaptureRow][ty] = '.';
    }
    if(piece == 'p' && tx == enpassantrow && ty == enpassantcol && target == '.' && abs(fy - ty) == 1 && fx + 1 == tx && board[fx][ty] == 'P')
    {
        enPassantCapture = true;
        enPassantCaptureRow = fx;
        enPassantCapturedPiece = board[enPassantCaptureRow][ty];
        board[enPassantCaptureRow][ty] = '.';
    }

    char captured = board[tx][ty];
    board[tx][ty] = piece;
    board[fx][fy] = '.';
    int oldwhitekingrow = whitekingrow, oldwhitekingcol = whitekingcol;
    int oldblackkingrow = blackkingrow, oldblackkingcol = blackkingcol;
    if(piece == 'K') {
        whitekingrow = tx;
        whitekingcol = ty;
    }
    if(piece == 'k') {
        blackkingrow = tx;
        blackkingcol = ty;
    }
    bool illegal=false;
    if(ischeck(isupper(piece)?1:0)){
        illegal=true;
    }
    board[fx][fy] = piece;
    board[tx][ty] = captured;
    if(enPassantCapture)
    {
        board[enPassantCaptureRow][ty] = enPassantCapturedPiece;
    }
    whitekingrow = oldwhitekingrow;
    whitekingcol = oldwhitekingcol;
    blackkingrow = oldblackkingrow;
    blackkingcol = oldblackkingcol;
    return !illegal;
    
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
    if (tx == fx - 1 && abs(fy - ty) == 1 && tx == enpassantrow && ty == enpassantcol && board[tx][ty] == '.' && board[fx][ty] == 'p') {
        return true; // En passant capture
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
    if (tx == fx + 1 && abs(fy - ty) == 1 && tx == enpassantrow && ty == enpassantcol && board[tx][ty] == '.' && board[fx][ty] == 'P') {
        return true; // En passant capture
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
    if(abs(fx - tx) <= 1 && abs(fy - ty) <= 1) {
        return true; // Normal king move
    }
    if(board[fx][fy]=='K'&&whitekingmoved==false){
        if(ty==6&&tx==7&&fy==4&&fx==7&&board[7][5]=='.'&&board[7][6]=='.'&&whitekingrookmoved==false&&whitekingmoved==false){
            return true;
        }
        if(ty==2&&tx==7&&fy==4&&fx==7&&board[7][3]=='.'&&board[7][2]=='.'&&board[7][1]=='.'&&whitequeenrookmoved==false&&whitekingmoved==false){
            return true;
        }
    }
    return false;
}
bool whitepawnattack(int fx, int fy, int tx, int ty) {
    return (tx == fx - 1 && abs(fy - ty) == 1);
}
bool blackpawnattack(int fx, int fy, int tx, int ty) {
    return (tx == fx + 1 && abs(fy - ty) == 1);
}
bool pieceattack(int fx, int fy, int tx, int ty) {
    char piece = board[fx][fy];
    if(piece=='P'||piece=='p'){
        return (isupper(piece) && whitepawnattack(fx, fy, tx, ty)) || (islower(piece) && blackpawnattack(fx, fy, tx, ty));
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
    return false;
}
int kingrow, kingcol;
bool ischeck(int side) {
    // Implement check detection logic here
    if(side == 1) {
        // Check if White's king is in check
        kingrow = whitekingrow;
        kingcol = whitekingcol;
    } else {
        // Check if Black's king is in check
        kingrow = blackkingrow;
        kingcol = blackkingcol;
    }

    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
            //check for every enemy's piece, whether it attack the king or not
            if(side==0 && isupper(board[i][j]) && board[i][j]!='.'){
                if(pieceattack(i,j,kingrow,kingcol)){
                    return true;
                }
            }
            if(side==1 && islower(board[i][j]) && board[i][j]!='.'){
                if(pieceattack(i,j,kingrow,kingcol)){
                    return true;
                }
            }
        }
    }
    return false; // Placeholder for check detection
}
std::vector<Move> generatemoves(int side) {
    std::vector<Move> moves;
    for(int i=0;i<8;i++){//i,j is from ; x,y is to
        for(int j=0;j<8;j++){
            char piece=board[i][j];
            if(piece=='.')
                continue;
            if(side==1&&!isupper(piece))
                continue;
            if(side==0&&isupper(piece))
                continue;
            for(int x=0;x<8;x++){
                for(int y=0;y<8;y++){
                    if(x==i&&y==j)
                        continue;
                    if(islegalmove(i,j,x,y)){
                        Move m;
                        m.fx=i;
                        m.fy=j;
                        m.tx=x;
                        m.ty=y;
                        m.cpiece=board[x][y];
                        if((piece == 'P' && x == 0) || (piece == 'p' && x == 7))
                        {
                            m.promotion = true;
                        }
                        moves.push_back(m);
                    }
                }
            }
        }
    }
    return moves;
}
void makemove(Move &m){
    char piece=board[m.fx][m.fy];
    const bool requestedPromotion = m.promotion;
    const char requestedPromotedPiece = m.promotedpiece;
    m.movedpiece = piece;
    m.castling = false;
    m.enpassant = false;
    m.promotion = requestedPromotion;
    m.promotedpiece = requestedPromotedPiece;
    m.prevenpassantrow=enpassantrow;
    m.prevenpassantcol=enpassantcol;
    if(piece=='k'||piece=='K'){
        m.prevkingmoved=(isupper(piece)?whitekingmoved:blackkingmoved);
        m.prevkingrookmoved=(isupper(piece)?whitekingrookmoved:blackkingrookmoved);
        m.prevqueenrookmoved=(isupper(piece)?whitequeenrookmoved:blackqueenrookmoved);
    }
     if(piece=='R'||piece=='r'){
        // queenside rook
        if((isupper(piece) && m.fx==7 && m.fy==0) || (islower(piece) && m.fx==0 && m.fy==0))
            m.prevqueenrookmoved=(isupper(piece)?whitequeenrookmoved:blackqueenrookmoved);
        if((isupper(piece) && m.fx==7 && m.fy==7) || (islower(piece) && m.fx==0 && m.fy==7))
            m.prevkingrookmoved=(isupper(piece)?whitekingrookmoved:blackkingrookmoved);
    }
    enpassantcol=-1;
    enpassantrow=-1;

    if(piece == 'P' && m.tx == m.prevenpassantrow && m.ty == m.prevenpassantcol && m.cpiece == '.' && abs(m.ty - m.fy) == 1 && board[m.fx][m.ty] == 'p')
    {
        m.enpassant = true;
        m.cpiece = board[m.fx][m.ty];
        board[m.fx][m.ty] = '.';
    }
    if(piece == 'p' && m.tx == m.prevenpassantrow && m.ty == m.prevenpassantcol && m.cpiece == '.' && abs(m.ty - m.fy) == 1 && board[m.fx][m.ty] == 'P')
    {
        m.enpassant = true;
        m.cpiece = board[m.fx][m.ty];
        board[m.fx][m.ty] = '.';
    }

    board[m.tx][m.ty]=piece;
    board[m.fx][m.fy]='.';
    if(piece=='K'){ 
        whitekingrow = m.tx;
        whitekingcol = m.ty;
        if(m.fx==7&&m.fy==4&&m.tx==7&&m.ty==6){
            m.castling=true;
            whitekingrookmoved=true;
            board[7][5]='R';
            board[7][7]='.';
        }
        if(m.fx==7&&m.fy==4&&m.tx==7&&m.ty==2){
            m.castling=true;
            whitequeenrookmoved=true;
            board[7][3]='R';
            board[7][0]='.';
        }
        whitekingmoved=true;
    }
    if(piece=='k'){ 
        blackkingrow = m.tx;
        blackkingcol = m.ty;
        if(m.fx==0&&m.fy==4&&m.tx==0&&m.ty==6){
            m.castling=true;
            blackkingrookmoved=true;
            board[0][5]='r';
            board[0][7]='.';
        }
        if(m.fx==0&&m.fy==4&&m.tx==0&&m.ty==2){
            m.castling=true;
            blackqueenrookmoved=true;
            board[0][3]='r';
            board[0][0]='.';
        }
        blackkingmoved=true;
    }
    if(piece=='R'){

    // queenside rook
    if(m.fx==7 && m.fy==0)
        whitequeenrookmoved=true;

    // kingside rook
    if(m.fx==7 && m.fy==7)
        whitekingrookmoved=true;
    }
    if(piece=='r'){
        // queenside rook
        if(m.fx==0 && m.fy==0)
            blackqueenrookmoved=true;

        // kingside rook
        if(m.fx==0 && m.fy==7)
            blackkingrookmoved=true;
    }
    if(m.tx==7&&m.ty==0&&m.cpiece=='R'){
        whitequeenrookmoved=true;
    }
    if(m.tx==7&&m.ty==7&&m.cpiece=='R'){
        whitekingrookmoved=true;
    }
    if(m.tx==0&&m.ty==0&&m.cpiece=='r'){
    
        blackqueenrookmoved=true;
    }
    if(m.tx==0&&m.ty==7&&m.cpiece=='r'){
        blackkingrookmoved=true;
    }
    if(piece=='P'&&m.fx==6&&m.tx==4){
        enpassantrow=5;
        enpassantcol=m.fy;
    }
    if(piece=='p'&&m.fx==1&&m.tx==3){
        enpassantrow=2;
        enpassantcol=m.fy;
    }
    if(piece == 'P' && m.tx == 0)
    {
        m.promotion = true;
        if(m.promotedpiece == '.' || !isupper(static_cast<unsigned char>(m.promotedpiece)))
        {
            m.promotedpiece = 'Q';
        }
        board[m.tx][m.ty] = m.promotedpiece;
    }
    if(piece == 'p' && m.tx == 7)
    {
        m.promotion = true;
        if(m.promotedpiece == '.' || !islower(static_cast<unsigned char>(m.promotedpiece)))
        {
            m.promotedpiece = 'q';
        }
        board[m.tx][m.ty] = m.promotedpiece;
    }
}
void undomove(Move &m){
    char piece=m.movedpiece;
    board[m.fx][m.fy]=piece;
    board[m.tx][m.ty]=m.cpiece;
    if(m.enpassant)
    {
        board[m.tx][m.ty]='.';
        board[m.fx][m.ty]=m.cpiece;
    }
    if(piece=='K'){ 
        whitekingrow = m.fx;
        whitekingcol = m.fy;
        if(m.castling && m.fx==7&&m.fy==4&&m.tx==7&&m.ty==6){
            board[7][5]='.';
            board[7][7]='R';
        }
        if(m.castling&& m.fx==7&&m.fy==4&&m.tx==7&&m.ty==2){
            board[7][3]='.';
            board[7][0]='R';
        }
    }
    if(piece=='k'){ 
        blackkingrow = m.fx;
        blackkingcol = m.fy;
        if(m.castling&& m.fx==0&&m.fy==4&&m.tx==0&&m.ty==6){
            board[0][5]='.';
            board[0][7]='r';
        }
        if(m.castling&& m.fx==0&&m.fy==4&&m.tx==0&&m.ty==2){
            board[0][3]='.';
            board[0][0]='r';
        }
    }
    if(piece=='K'||piece=='k'){
        if(isupper(piece)){
            whitekingmoved=m.prevkingmoved;
            whitekingrookmoved=m.prevkingrookmoved;
            whitequeenrookmoved=m.prevqueenrookmoved;
        }
        else{
            blackkingmoved=m.prevkingmoved;
            blackkingrookmoved=m.prevkingrookmoved;
            blackqueenrookmoved=m.prevqueenrookmoved;
        }
    }
    if(piece=='R'||piece=='r'){
        if(isupper(piece)){
            if(m.fx==7 && m.fy==0)
                whitequeenrookmoved=m.prevqueenrookmoved;
            if(m.fx==7 && m.fy==7)
                whitekingrookmoved=m.prevkingrookmoved;
        }
        else{
            if(m.fx==0 && m.fy==0)
                blackqueenrookmoved=m.prevqueenrookmoved;
            if(m.fx==0 && m.fy==7)
                blackkingrookmoved=m.prevkingrookmoved;
        }
    }
    enpassantrow=m.prevenpassantrow;
    enpassantcol=m.prevenpassantcol;
}
int evaluate(){
    int score=0;
    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
            char piece=board[i][j];
            if(piece=='.')
                continue;
            int value=0;
            switch(toupper(piece)){
                case 'P': value=100; break;
                case 'N': value=320; break;
                case 'B': value=330; break;
                case 'R': value=500; break;
                case 'Q': value=900; break;
                case 'K': value=200000; break;
            }
            score+=(isupper(piece)?value:-value);
        }
    }
    return score;
}

namespace {
constexpr int kMateScore = 1000000;

vector<char> promotionPieces(bool white)
{
    return white ? vector<char>{'Q', 'R', 'B', 'N'}
                 : vector<char>{'q', 'r', 'b', 'n'};
}
}
int piecevalue(char piece){
    int value=0;
    switch(toupper(piece)){
        case 'P': value=100; break;
        case 'N': value=320; break;
        case 'B': value=330; break;
        case 'R': value=500; break;
        case 'Q': value=900; break;
        case 'K': value=200000; break;
    }
    return value;
}
int scoremove(Move &m){
    int score=0;
    char attacker = board[m.fx][m.fy];
    char target = m.cpiece;
    if(target != '.')
    {
        score+=100000;
        score += 10 * piecevalue(target) - piecevalue(attacker);
    }
     if(m.promotion)
    {
        score+=80000;
        score += piecevalue(m.promotedpiece) - piecevalue(attacker);
    }
     if(m.castling)
    {
        score += 50; // Arbitrary bonus for castling
    }
    return score;
}
int alphabeta(int depth,int alpha, int beta, bool white){
    vector<Move> moves = generatemoves(white);
    for(Move &m : moves) {
        m.score = scoremove(m);
    }
    sort(moves.begin(), moves.end(), [](const Move &a, const Move &b) {
        return a.score > b.score; // Sort in descending order of score
    });
    if(moves.empty()){
        if(ischeck(white?1:0)){
            // Faster mates are preferred by keeping score farther from zero.
            return white ? (-kMateScore + depth) : (kMateScore - depth);
        }
        return 0; // Stalemate
    }
    if(depth==0){
        return evaluate();
    }

    if(white){
        int maxi = -kMateScore - 1;
        for(Move &m : moves){
            if(m.promotion && m.promotedpiece == '.'){
                int bestPromotionScore = -kMateScore - 1;
                for(char promotionPiece : promotionPieces(true)){
                    Move trial = m;
                    trial.promotedpiece = promotionPiece;
                    makemove(trial);
                    int score = alphabeta(depth-1, alpha, beta, false);
                    undomove(trial);
                    bestPromotionScore = max(bestPromotionScore, score);
                    alpha = max(alpha, bestPromotionScore);
                    if(beta <= alpha){
                        break;
                    }
                }
                maxi = max(maxi, bestPromotionScore);
                if(beta <= alpha){
                    break;
                }
            } else {
                makemove(m);
                int score = alphabeta(depth-1, alpha, beta, false);
                undomove(m);
                maxi = max(maxi, score);
                alpha = max(alpha, score);
                if(beta <= alpha){
                    break;
                }
            }
        }
        return maxi;
    } 
    else {
        int mini = kMateScore + 1;
        for(Move &m : moves){
            if(m.promotion && m.promotedpiece == '.'){
                int bestPromotionScore = kMateScore + 1;
                for(char promotionPiece : promotionPieces(false)){
                    Move trial = m;
                    trial.promotedpiece = promotionPiece;
                    makemove(trial);
                    int score = alphabeta(depth-1, alpha, beta, true);
                    undomove(trial);
                    bestPromotionScore = min(bestPromotionScore, score);
                    beta = min(beta, bestPromotionScore);
                    if(beta <= alpha){
                        break;
                    }
                }
                mini = min(mini, bestPromotionScore);
                if(beta <= alpha){
                    break;
                }
            } else {
                makemove(m);
                int score = alphabeta(depth-1, alpha, beta, true);
                undomove(m);
                mini = min(mini,score);
                beta = min(beta, score);
                if(beta <= alpha){
                    break;
                }
            }
        }
        return mini;
    }
}
Move findbestmove(bool white,int depth){
    vector<Move> moves = generatemoves(white);
    if(moves.empty()){
        Move m;
        m.fx=-1; // No move available
        return m;
    }
    Move bestmove = moves[0];
    int bestscore = white ? (-kMateScore - 1) : (kMateScore + 1);
    for(Move &m : moves){
        if(m.promotion && m.promotedpiece == '.'){
            for(char promotionPiece : promotionPieces(white)){
                Move trial = m;
                trial.promotedpiece = promotionPiece;
                makemove(trial);
                int score = alphabeta(depth-1, -kMateScore - 1, kMateScore + 1, !white);
                undomove(trial);
                if((white && score > bestscore) || (!white && score < bestscore)){
                    bestscore = score;
                    bestmove = trial;
                }
            }
        } else {
            makemove(m);
            int score = alphabeta(depth-1, -kMateScore - 1, kMateScore + 1, !white);
            undomove(m);
            if((white && score > bestscore) || (!white && score < bestscore)){
                bestscore = score;
                bestmove = m;
            }
        }
    }
    return bestmove;
}