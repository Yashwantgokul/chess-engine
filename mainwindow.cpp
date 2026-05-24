// ============================================
// mainwindow.cpp
// ============================================

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "board.h"
#include "movegen.h"
#include <QCoreApplication>
#include <QDir>
#include <QDirIterator>
#include <QIcon>
#include <QMessageBox>
#include <QGridLayout>
#include <QHash>
#include <QPushButton>
#include <QSize>

namespace {

QString findPieceIconFile(char piece)
{
    static QHash<char, QString> cachedPaths;

    const auto cached = cachedPaths.constFind(piece);
    if(cached != cachedPaths.constEnd())
    {
        return cached.value();
    }

    const QString iconName = [piece]() -> QString {
        switch(piece)
        {
        case 'K': return "wk.svg";
        case 'Q': return "wq.svg";
        case 'R': return "wr.svg";
        case 'B': return "wb.svg";
        case 'N': return "wn.svg";
        case 'P': return "wp.svg";
        case 'k': return "bk.svg";
        case 'q': return "bq.svg";
        case 'r': return "br.svg";
        case 'b': return "bb.svg";
        case 'n': return "bn.svg";
        case 'p': return "bp.svg";
        default: return QString();
        }
    }();

    if(iconName.isEmpty())
    {
        return QString();
    }

    QStringList searchRoots;
    QDir directory(QCoreApplication::applicationDirPath());

    for(int i = 0; i < 5 && directory.exists(); ++i)
    {
        searchRoots.append(directory.absolutePath());
        if(!directory.cdUp())
        {
            break;
        }
    }

    searchRoots.append(QDir::currentPath());

    for(const QString &rootPath : searchRoots)
    {
        QDirIterator iterator(rootPath,
                              QStringList() << iconName,
                              QDir::Files,
                              QDirIterator::Subdirectories);

        if(iterator.hasNext())
        {
            const QString resolvedPath = iterator.next();
            cachedPaths.insert(piece, resolvedPath);
            return resolvedPath;
        }
    }

    cachedPaths.insert(piece, QString());
    return QString();
}

}
// ============================================
// CONSTRUCTOR
// ============================================
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // INITIALIZE ENGINE BOARD
    initializeBoard();

    // CREATE GUI
    createBoard();

    // DRAW PIECES
    drawBoard();

    setWindowTitle("Chess Engine");

    setFixedSize(8 * 80, 8 * 80);
}

// ============================================
// DESTRUCTOR
// ============================================

MainWindow::~MainWindow()
{
    delete ui;
}

// ============================================
// CREATE BOARD
// ============================================

void MainWindow::createBoard()
{
    QWidget *centralWidget = new QWidget(this);

    setCentralWidget(centralWidget);

    QGridLayout *grid = new QGridLayout();

    // REMOVE SPACES
    grid->setSpacing(0);

    // REMOVE OUTER MARGINS
    grid->setContentsMargins(0,0,0,0);

    centralWidget->setLayout(grid);

    for(int row = 0; row < 8; row++)
    {
        for(int col = 0; col < 8; col++)
        {
            QPushButton *button = new QPushButton();

            button->setFixedSize(80,80);
            button->setIconSize(QSize(64, 64));

            // STORE BUTTON
            squares[row][col] = button;

            // ADD TO GRID
            grid->addWidget(button,row,col);

            // CONNECT CLICK
            connect(button,
                    &QPushButton::clicked,
                    this,
                    &MainWindow::handleSquareClick);
        }
    }

    resetBoardColors();
}

// ============================================
// RESET COLORS
// ============================================

void MainWindow::resetBoardColors()
{
    for(int row = 0; row < 8; row++)
    {
        for(int col = 0; col < 8; col++)
        {
            if((row + col) % 2 == 0)
            {
                // LIGHT
                squares[row][col]->setStyleSheet(
                    "background-color: #F0D9B5;"
                    "border:none;"
                    );
            }
            else
            {
                // DARK
                squares[row][col]->setStyleSheet(
                    "background-color: #B58863;"
                    "border:none;"
                    );
            }
        }
    }
}

// ============================================
// DRAW BOARD
// ============================================

void MainWindow::drawBoard()
{
    for(int row = 0; row < 8; row++)
    {
        for(int col = 0; col < 8; col++)
        {
            QPushButton *button = squares[row][col];
            const QString iconPath = getPieceIconPath(board[row][col]);

            button->setText(QString());
            button->setIcon(QIcon());

            if(!iconPath.isEmpty())
            {
                button->setIcon(QIcon(iconPath));
            }
            else
            {
                button->setText(getPieceUnicode(board[row][col]));
            }
        }
    }
}

// ============================================
// PIECE TO UNICODE
// ============================================

QString MainWindow::getPieceUnicode(char piece)
{
    switch(piece)
    {
    case 'K': return "♔";
    case 'Q': return "♕";
    case 'R': return "♖";
    case 'B': return "♗";
    case 'N': return "♘";
    case 'P': return "♙";

    case 'k': return "♚";
    case 'q': return "♛";
    case 'r': return "♜";
    case 'b': return "♝";
    case 'n': return "♞";
    case 'p': return "♟";

    default: return "";
    }
}

// ============================================
// PIECE TO ICON PATH
// ============================================

QString MainWindow::getPieceIconPath(char piece)
{
    return findPieceIconFile(piece);
}

// ============================================
// HANDLE CLICK
// ============================================

void MainWindow::handleSquareClick()
{
    QPushButton *clickedButton =
        qobject_cast<QPushButton*>(sender());

    int row = -1;
    int col = -1;

    // FIND BUTTON
    for(int r = 0; r < 8; r++)
    {
        for(int c = 0; c < 8; c++)
        {
            if(squares[r][c] == clickedButton)
            {
                row = r;
                col = c;
            }
        }
    }
    // ========================================
    // FIRST CLICK
    // ========================================

    if(selectedRow == -1)
    {
        // EMPTY SQUARE
        if(board[row][col] == '.')
        {
            return;
        }

        selectedRow = row;
        selectedCol = col;

        // HIGHLIGHT
        squares[row][col]->setStyleSheet(
            "background-color: yellow;"
            "border:none;"
            );
    }

    // ========================================
    // SECOND CLICK
    // ========================================

    else
    {
        // LEGAL MOVE CHECK
        if(islegalmove(
                selectedRow,
                selectedCol,
                row,
                col))
        {
            Move m;

            m.fx = selectedRow;
            m.fy = selectedCol;

            m.tx = row;
            m.ty = col;

            m.cpiece = board[row][col];
            // PLAYER MOVE
            makemove(m);
            drawBoard();
            // COMPUTER MOVE
            Move bestmove = findbestmove(false,3);
            if(bestmove.fx == -1)
            {
                // No legal move for black: checkmate or stalemate
                if(ischeck(0)){
                    QMessageBox::information(this, "Game Over", "Checkmate — You win!");
                } else {
                    QMessageBox::information(this, "Game Over", "Stalemate — Draw.");
                }
                selectedRow = -1;
                selectedCol = -1;
                resetBoardColors();
                return;
            }
            else
            {
                makemove(bestmove);
                drawBoard();
                // After engine move, check if player has any legal replies
                auto playerMoves = generatemoves(true);
                if(playerMoves.empty()){
                    if(ischeck(1)){
                        QMessageBox::information(this, "Game Over", "Checkmate — You lose.");
                    } else {
                        QMessageBox::information(this, "Game Over", "Stalemate — Draw.");
                    }
                    selectedRow = -1;
                    selectedCol = -1;
                    resetBoardColors();
                    return;
                }
            }
        }
        // RESET
        selectedRow = -1;
        selectedCol = -1;

        resetBoardColors();
    }
}