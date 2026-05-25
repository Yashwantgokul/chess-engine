// ============================================
// mainwindow.h
// ============================================

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QString>
#include <QFutureWatcher>
#include <QTimer>
#include <QSoundEffect>
#include <QVector>
#include <QAudioDevice>
#include <QMediaDevices>

struct Move;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

// ============================================
// SOUND POOL
// A pool of QSoundEffect instances for one sound type.
// Finds the next idle (non-playing) instance so sounds
// never interfere with each other — exactly like chess.com.
// ============================================

struct SoundPool
{
    static constexpr int kPoolSize = 6;

    void init(const QString &filePath, QObject *parent);
    void play();
    void setAudioDevice(const QAudioDevice &device);

    QVector<QSoundEffect*> players;
    int next = 0;
};

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

    bool hasLastComputerMove = false;

    int computerFromRow = -1;
    int computerFromCol = -1;
    int computerToRow   = -1;
    int computerToCol   = -1;

    int checkedKingRow = -1;
    int checkedKingCol = -1;

    bool checkFlashOn = false;
    QTimer *checkFlashTimer = nullptr;

    bool computerTurnPending = false;
    int  computerMoveDelayMs = 450;

    QFutureWatcher<Move> *computerMoveWatcher = nullptr;

    // ============================================
    // AUDIO — one pool per sound type
    // ============================================

    SoundPool moveSoundPool;
    SoundPool captureSoundPool;
    SoundPool checkSoundPool;
    QMediaDevices *mediaDevices = nullptr;

    // ============================================
    // FUNCTIONS
    // ============================================

    void createBoard();
    void drawBoard();
    void resetBoardColors();
    void updateCheckState();
    void updateBoardHighlights();
    void initializeSounds();
    void refreshAudioOutputDevice();
    void runComputerTurn();
    void applyComputerMove(Move &bestmove);
    void playMoveSound(char capturedPiece, bool gaveCheck);
    QString getPieceUnicode(char piece);
    QString getPieceIconPath(char piece);
};

#endif // MAINWINDOW_H