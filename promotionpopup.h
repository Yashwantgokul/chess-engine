#ifndef PROMOTIONPOPUP_H
#define PROMOTIONPOPUP_H

#include <QDialog>
#include <QPoint>

class QPushButton;

class PromotionPopup : public QDialog
{
    Q_OBJECT

public:
    explicit PromotionPopup(bool isWhitePiece, QWidget *parent = nullptr);

    // Shows the popup near a square and blocks until the user picks a piece.
    // Returns the chosen piece char, or '.' if cancelled.
    char execNearSquare(const QPoint &squareTopLeftGlobal, int squareSize, bool showAbove);

private:
    void createButtonRow(bool isWhitePiece);

    char m_chosen = '.';
};

#endif // PROMOTIONPOPUP_H