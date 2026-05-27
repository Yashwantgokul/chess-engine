#include "promotionpopup.h"

#include <QApplication>
#include <QCoreApplication>
#include <QColor>
#include <QDir>
#include <QDirIterator>
#include <QGraphicsDropShadowEffect>
#include <QGuiApplication>
#include <QHBoxLayout>
#include <QIcon>
#include <QPushButton>
#include <QScreen>
#include <QSize>
#include <QStringList>
#include <QVector>

namespace {

QString findPromotionPieceIconFile(char piece)
{
    const QString iconName = [piece]() -> QString {
        switch(piece)
        {
        case 'Q': case 'q': return piece == 'Q' ? "wq.svg" : "bq.svg";
        case 'R': case 'r': return piece == 'R' ? "wr.svg" : "br.svg";
        case 'B': case 'b': return piece == 'B' ? "wb.svg" : "bb.svg";
        case 'N': case 'n': return piece == 'N' ? "wn.svg" : "bn.svg";
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
            return iterator.next();
        }
    }

    return QString();
}

QString pieceLabel(char piece)
{
    switch(piece)
    {
    case 'Q': return QStringLiteral("♕");
    case 'R': return QStringLiteral("♖");
    case 'B': return QStringLiteral("♗");
    case 'N': return QStringLiteral("♘");
    case 'q': return QStringLiteral("♛");
    case 'r': return QStringLiteral("♜");
    case 'b': return QStringLiteral("♝");
    case 'n': return QStringLiteral("♞");
    default: return QString();
    }
}

} // namespace

// ============================================
// PromotionPopup — QDialog-based
// ============================================

PromotionPopup::PromotionPopup(bool isWhitePiece, QWidget *parent)
    : QDialog(parent,
              Qt::FramelessWindowHint |
              Qt::WindowStaysOnTopHint)
{
    setObjectName("promotionPopup");
    setAttribute(Qt::WA_StyledBackground, true);

    // QDialog is modal by default when exec() is called.
    setWindowModality(Qt::ApplicationModal);
    setFixedSize(180, 50);

    auto *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setBlurRadius(18);
    shadow->setOffset(0, 4);
    shadow->setColor(QColor(0, 0, 0, 90));
    setGraphicsEffect(shadow);

    setStyleSheet(
        "#promotionPopup {"
        "background-color: #f0f0f0;"
        "border: 1px solid black;"
        "border-radius: 6px;"
        "}"
        "QPushButton {"
        "background-color: transparent;"
        "border: none;"
        "border-radius: 5px;"
        "font-size: 24px;"
        "padding: 0px;"
        "}"
        "QPushButton:hover {"
        "background-color: #dcdcdc;"
        "}"
    );

    createButtonRow(isWhitePiece);
}

void PromotionPopup::createButtonRow(bool isWhitePiece)
{
    auto *layout = new QHBoxLayout(this);
    layout->setContentsMargins(6, 6, 6, 6);
    layout->setSpacing(4);

    const QVector<char> pieces = isWhitePiece
            ? QVector<char>{'Q', 'R', 'B', 'N'}
            : QVector<char>{'q', 'r', 'b', 'n'};

    for(char piece : pieces)
    {
        auto *button = new QPushButton(pieceLabel(piece), this);
        button->setFixedSize(40, 40);
        button->setCursor(Qt::PointingHandCursor);

        const QString iconPath = findPromotionPieceIconFile(piece);
        if(!iconPath.isEmpty())
        {
            button->setText(QString());
            button->setIcon(QIcon(iconPath));
            button->setIconSize(QSize(28, 28));
        }

        connect(button, &QPushButton::clicked, this, [this, piece]() {
            m_chosen = piece;
            accept(); // closes the dialog with Accepted result
        });

        layout->addWidget(button);
    }
}

char PromotionPopup::execNearSquare(const QPoint &squareTopLeftGlobal, int squareSize, bool showAbove)
{
    m_chosen = '.';

    const QPoint squareCenterGlobal(
        squareTopLeftGlobal.x() + squareSize / 2,
        squareTopLeftGlobal.y() + squareSize / 2);

    const QScreen *screen = QGuiApplication::screenAt(squareCenterGlobal);
    if(screen == nullptr)
    {
        screen = QGuiApplication::primaryScreen();
    }

    const QRect availableGeometry = screen != nullptr
            ? screen->availableGeometry()
            : QRect();

    int popupX = squareTopLeftGlobal.x() + (squareSize - width()) / 2;
    const int belowY = squareTopLeftGlobal.y() + squareSize + 4;
    const int aboveY = squareTopLeftGlobal.y() - height() - 4;

    int popupY = showAbove ? aboveY : belowY;

    if(screen != nullptr)
    {
        // Flip if it would go off screen
        if(showAbove && aboveY < availableGeometry.top())
        {
            popupY = belowY;
        }
        else if(!showAbove && belowY + height() > availableGeometry.bottom())
        {
            popupY = aboveY;
        }

        const int minX = availableGeometry.left();
        const int maxX = availableGeometry.right() - width();
        popupX = qBound(minX, popupX, maxX);

        const int minY = availableGeometry.top();
        const int maxY = availableGeometry.bottom() - height();
        popupY = qBound(minY, popupY, maxY);
    }

    move(popupX, popupY);
    exec(); // blocks until accept() or reject() is called

    return m_chosen;
}