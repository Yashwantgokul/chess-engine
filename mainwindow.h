// ============================================
// mainwindow.h
// ============================================

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

private slots:

    void handleSquareClick();

private:

    Ui::MainWindow *ui;

    // ============================================
    // GUI BUTTON BOARD
    // ============================================

    QPushButton* squares[8][8];

    // ============================================
    // SELECTED SQUARE
    // ============================================

    int selectedRow = -1;

    int selectedCol = -1;

    // ============================================
    // FUNCTIONS
    // ============================================

    void createBoard();

    void drawBoard();

    void resetBoardColors();

    QString getPieceUnicode(char piece);

    QString getPieceIconPath(char piece);
};

#endif // MAINWINDOW_H