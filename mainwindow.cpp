// ============================================
// mainwindow.cpp
// ============================================

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "promotionpopup.h"
#include "board.h"
#include "movegen.h"
#include <QCoreApplication>
#include <QDir>
#include <QDirIterator>
#include <QFile>
#include <QIcon>
#include <QMessageBox>
#include <QGridLayout>
#include <QHash>
#include <QEventLoop>
#include <QMediaDevices>
#include <QPushButton>
#include <QSize>
#include <QSoundEffect>
#include <QTimer>
#include <QtConcurrent>
#include <cctype>

// ============================================
// SOUND POOL — implementation
// ============================================

void SoundPool::init(const QString &filePath, QObject *parent)
{
    if(filePath.isEmpty()) return;

    const QUrl url = QUrl::fromLocalFile(filePath);
    players.reserve(kPoolSize);

    for(int i = 0; i < kPoolSize; ++i)
    {
        QSoundEffect *sfx = new QSoundEffect(parent);
        sfx->setSource(url);
        sfx->setVolume(1.0f);
        sfx->setLoopCount(1);
        players.append(sfx);
    }
}

void SoundPool::play()
{
    if(players.isEmpty()) return;

    const QAudioDevice currentOutput = QMediaDevices::defaultAudioOutput();

    // Walk the pool to find a player that has finished or never started.
    for(int i = 0; i < players.size(); ++i)
    {
        QSoundEffect *sfx = players[(next + i) % players.size()];
        if(!sfx->isPlaying())
        {
            sfx->setAudioDevice(currentOutput);
            sfx->play();
            next = (next + i + 1) % players.size();
            return;
        }
    }

    // All instances are playing (very rapid moves) — use round-robin.
    // QSoundEffect::play() on an already-playing instance restarts it
    // cleanly without volume interference.
    players[next]->setAudioDevice(currentOutput);
    players[next]->play();
    next = (next + 1) % players.size();
}

void SoundPool::setAudioDevice(const QAudioDevice &device)
{
    for(QSoundEffect * &sfx : players)
    {
        sfx->setAudioDevice(device);
    }
}

namespace {

QString findRuntimeFile(const QString &fileName)
{
    QDir appDir(QCoreApplication::applicationDirPath());
    for(int i = 0; i < 6; ++i)
    {
        const QString candidate = appDir.filePath(fileName);
        if(QFile::exists(candidate))
        {
            return candidate;
        }

        if(!appDir.cdUp())
        {
            break;
        }
    }

    QDir cwd(QDir::currentPath());
    for(int i = 0; i < 6; ++i)
    {
        const QString candidate = cwd.filePath(fileName);
        if(QFile::exists(candidate))
        {
            return candidate;
        }

        if(!cwd.cdUp())
        {
            break;
        }
    }

    return QString();
}

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

    QMessageBox modeDialog(this);
    modeDialog.setWindowTitle("Game Mode");
    modeDialog.setText("Choose a mode before starting:");
    QPushButton *vsComputerButton = modeDialog.addButton("Play vs Computer", QMessageBox::AcceptRole);
    QPushButton *twoPlayerButton = modeDialog.addButton("2 Player", QMessageBox::ActionRole);
    modeDialog.setDefaultButton(vsComputerButton);
    modeDialog.exec();

    playVsComputer = (modeDialog.clickedButton() != twoPlayerButton);

    // INITIALIZE ENGINE BOARD
    initializeBoard();

    // CREATE GUI
    createBoard();

    // DRAW PIECES
    drawBoard();
    initializeSounds();

    checkFlashTimer = new QTimer(this);
    checkFlashTimer->setInterval(350);
    connect(checkFlashTimer, &QTimer::timeout, this, [this]() {
        checkFlashOn = !checkFlashOn;
        updateBoardHighlights();
    });

    updateCheckState();
    updateBoardHighlights();

    setWindowTitle(playVsComputer ? "Chess Engine - Vs Computer" : "Chess Engine - 2 Player");

    setFixedSize(8 * 80, 8 * 80);
}

// ============================================
// SOUND
// ============================================

void MainWindow::initializeSounds()
{
    QString moveSoundPath = findRuntimeFile("move-self.wav");
    if(moveSoundPath.isEmpty()) moveSoundPath = findRuntimeFile("move-self.mp3");

    QString captureSoundPath = findRuntimeFile("capture.wav");
    if(captureSoundPath.isEmpty()) captureSoundPath = findRuntimeFile("capture.mp3");

    QString checkSoundPath = findRuntimeFile("move-check.wav");
    if(checkSoundPath.isEmpty()) checkSoundPath = findRuntimeFile("move-check.mp3");

    moveSoundPool.init(moveSoundPath, this);
    captureSoundPool.init(captureSoundPath, this);
    checkSoundPool.init(checkSoundPath, this);

    mediaDevices = new QMediaDevices(this);
    connect(mediaDevices, &QMediaDevices::audioOutputsChanged, this, [this]() {
        refreshAudioOutputDevice();
    });

    refreshAudioOutputDevice();
}

void MainWindow::refreshAudioOutputDevice()
{
    const QAudioDevice selectedOutput = QMediaDevices::defaultAudioOutput();

    moveSoundPool.setAudioDevice(selectedOutput);
    captureSoundPool.setAudioDevice(selectedOutput);
    checkSoundPool.setAudioDevice(selectedOutput);
}

void MainWindow::playMoveSound(char capturedPiece, bool gaveCheck)
{
    if(gaveCheck)
    {
        checkSoundPool.play();
    }
    else if(capturedPiece != '.')
    {
        captureSoundPool.play();
    }
    else
    {
        moveSoundPool.play();
    }
}

void MainWindow::runComputerTurn()
{
    if(computerMoveWatcher != nullptr)
    {
        return;
    }

    computerMoveWatcher = new QFutureWatcher<Move>(this);
    connect(computerMoveWatcher, &QFutureWatcher<Move>::finished, this, [this]() {
        if(computerMoveWatcher == nullptr)
        {
            return;
        }

        Move bestmove = computerMoveWatcher->result();
        computerMoveWatcher->deleteLater();
        computerMoveWatcher = nullptr;
        applyComputerMove(bestmove);
    });

    computerMoveWatcher->setFuture(QtConcurrent::run([]() {
        return findbestmove(false,3);
    }));
}

void MainWindow::applyComputerMove(Move &bestmove)
{
    if(bestmove.fx == -1)
    {
        if(ischeck(0)){
            QMessageBox::information(this, "Game Over", "Checkmate — You win!");
        } else {
            QMessageBox::information(this, "Game Over", "Stalemate — Draw.");
        }

        selectedRow = -1;
        selectedCol = -1;
        hasLastMove = false;
        updateCheckState();
        updateBoardHighlights();
        computerTurnPending = false;
        return;
    }

    makemove(bestmove);
    whiteToMove = true;
    drawBoard();

    hasLastMove = true;
    lastMoveFromRow = bestmove.fx;
    lastMoveFromCol = bestmove.fy;
    lastMoveToRow = bestmove.tx;
    lastMoveToCol = bestmove.ty;

    updateCheckState();
    updateBoardHighlights();
    playMoveSound(bestmove.cpiece, ischeck(1));

    auto playerMoves = generatemoves(true);
    if(playerMoves.empty()){
        if(ischeck(1)){
            QMessageBox::information(this, "Game Over", "Checkmate — You lose.");
        } else {
            QMessageBox::information(this, "Game Over", "Stalemate — Draw.");
        }
        selectedRow = -1;
        selectedCol = -1;
        updateCheckState();
        updateBoardHighlights();
    }

    computerTurnPending = false;
}
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
// CHECK STATE
// ============================================

void MainWindow::updateCheckState()
{
    checkedKingRow = -1;
    checkedKingCol = -1;

    if(ischeck(1))
    {
        checkedKingRow = whitekingrow;
        checkedKingCol = whitekingcol;
    }
    else if(ischeck(0))
    {
        checkedKingRow = blackkingrow;
        checkedKingCol = blackkingcol;
    }

    if(checkedKingRow != -1)
    {
        checkFlashOn = true;
        if(!checkFlashTimer->isActive())
        {
            checkFlashTimer->start();
        }
    }
    else
    {
        checkFlashOn = false;
        if(checkFlashTimer->isActive())
        {
            checkFlashTimer->stop();
        }
    }
}

// ============================================
// BOARD HIGHLIGHTS
// ============================================

void MainWindow::updateBoardHighlights()
{
    resetBoardColors();

    if(hasLastMove)
    {
        squares[lastMoveFromRow][lastMoveFromCol]->setStyleSheet(
            "background-color: #f7ea62;"
            "border:none;"
            );

        squares[lastMoveToRow][lastMoveToCol]->setStyleSheet(
            "background-color: #f2d64b;"
            "border:none;"
            );
    }

    if(selectedRow != -1 && selectedCol != -1)
    {
        squares[selectedRow][selectedCol]->setStyleSheet(
            "background-color: #f6f669;"
            "border:none;"
            );
    }

    if(checkedKingRow != -1 && checkedKingCol != -1)
    {
        const char *checkColor = checkFlashOn ? "#ef5350" : "#ff8a80";
        squares[checkedKingRow][checkedKingCol]->setStyleSheet(
            QString("background-color: %1;border:none;").arg(checkColor)
            );
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

char MainWindow::choosePromotionPiece(bool isWhitePiece, const QPoint &anchorGlobalPos, int squareSize)
{
    PromotionPopup popup(isWhitePiece, this);
    // White promotes at row 0 (top)   — show popup BELOW the square.
    // Black promotes at row 7 (bottom) — show popup ABOVE the square.
    const bool showAbove = !isWhitePiece;
    return popup.execNearSquare(anchorGlobalPos, squareSize, showAbove);
}

// ============================================
// HANDLE CLICK
// ============================================

void MainWindow::handleSquareClick()
{
    if(computerTurnPending)
    {
        return;
    }

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

        if(whiteToMove)
        {
            if(!std::isupper(static_cast<unsigned char>(board[row][col])))
            {
                return;
            }
        }
        else
        {
            if(!std::islower(static_cast<unsigned char>(board[row][col])))
            {
                return;
            }
        }

        selectedRow = row;
        selectedCol = col;
        updateBoardHighlights();
    }

    // ========================================
    // SECOND CLICK
    // ========================================

    else
    {
        if(whiteToMove)
        {
            if(!std::isupper(static_cast<unsigned char>(board[selectedRow][selectedCol])))
            {
                selectedRow = -1;
                selectedCol = -1;
                updateBoardHighlights();
                return;
            }
        }
        else
        {
            if(!std::islower(static_cast<unsigned char>(board[selectedRow][selectedCol])))
            {
                selectedRow = -1;
                selectedCol = -1;
                updateBoardHighlights();
                return;
            }
        }

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

            const char movingPiece = board[selectedRow][selectedCol];
            const bool needsPromotion =
                    (movingPiece == 'P' && row == 0) ||
                    (movingPiece == 'p' && row == 7);

            if(needsPromotion)
            {
                const QPoint popupPosition = squares[row][col]->mapToGlobal(QPoint(0, 0));
                const char promotedPiece = choosePromotionPiece(
                            std::isupper(static_cast<unsigned char>(movingPiece)),
                            popupPosition,
                            squares[row][col]->height());

                if(promotedPiece == '.')
                {
                    return;
                }

                m.promotion = true;
                m.promotedpiece = promotedPiece;
            }

            // PLAYER MOVE
            makemove(m);
            drawBoard();
            hasLastMove = true;
            lastMoveFromRow = m.fx;
            lastMoveFromCol = m.fy;
            lastMoveToRow = m.tx;
            lastMoveToCol = m.ty;
            updateCheckState();
            updateBoardHighlights();
            const bool gaveCheck = whiteToMove ? ischeck(0) : ischeck(1);
            playMoveSound(m.cpiece, gaveCheck);

            whiteToMove = !whiteToMove;

            selectedRow = -1;
            selectedCol = -1;
            updateBoardHighlights();

            const auto nextMoves = generatemoves(whiteToMove);
            if(nextMoves.empty())
            {
                if(ischeck(whiteToMove ? 1 : 0))
                {
                    if(playVsComputer)
                    {
                        if(whiteToMove)
                        {
                            QMessageBox::information(this, "Game Over", "Checkmate - You lose.");
                        }
                        else
                        {
                            QMessageBox::information(this, "Game Over", "Checkmate - You win!");
                        }
                    }
                    else
                    {
                        QMessageBox::information(this,
                                                 "Game Over",
                                                 whiteToMove ? "Checkmate - Black wins." : "Checkmate - White wins.");
                    }
                }
                else
                {
                    QMessageBox::information(this, "Game Over", "Stalemate - Draw.");
                }
                return;
            }

            if(!playVsComputer)
            {
                return;
            }

            computerTurnPending = true;
            QTimer::singleShot(computerMoveDelayMs, this, [this]() {
                runComputerTurn();
            });
            return;
        }
        // RESET
        selectedRow = -1;
        selectedCol = -1;
        updateBoardHighlights();
    }
}