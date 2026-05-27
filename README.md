# ♟️ ChessEngine
<img width="799" height="838" alt="image" src="https://github.com/user-attachments/assets/c778794a-5858-46ef-b062-f53a10068880" />

A modern chess engine and GUI application written in C++ using Qt and CMake.

This project was built as a deep exploration into how real chess engines work internally — from legal move generation and king safety validation to recursive AI search using Minimax and Alpha-Beta pruning.

The engine evolved from a basic board representation project into a playable chess application supporting:

* Intelligent AI search
* Full chess rules
* Recursive move simulation
* Move undo architecture
* GUI visualizations
* Sound effects
* Two-player gameplay

---

# ✨ Features

## ♟️ Chess Engine

* Minimax-based AI
* Alpha-Beta pruning optimization
* Recursive game tree search
* Position evaluation system
* Legal move generation
* King safety validation
* Check detection
* Checkmate detection
* Move simulation architecture
* makeMove / undoMove system
* Castling
* En passant
* Pawn promotion

---

## 🖥️ GUI Features

* Qt6-based graphical interface
* Piece icons
* Last move highlighting
* Sound effects
* Click-based piece movement
* 2-player local mode
* AI gameplay support

---

# 🧠 Concepts Implemented

This project was mainly built to understand core chess engine architecture and AI search systems.

## Algorithms & Techniques

* Minimax Algorithm
* Alpha-Beta Pruning
* Recursive Search Trees
* Position Evaluation Heuristics
* Legal Move Validation
* Move Simulation
* State Snapshot Systems
* Board Representation
* GUI ↔ Engine Synchronization

---

# 🏗️ Engine Architecture

```text
                             ┌──────────────────────┐
                             │      Qt6 GUI         │
                             │   (mainwindow.cpp)  │
                             └──────────┬───────────┘
                                        │
                                        ▼
                           ┌─────────────────────────┐
                           │      Board Manager      │
                           │       board.cpp         │
                           └──────────┬──────────────┘
                                      │
        ┌─────────────────────────────┼─────────────────────────────┐
        ▼                             ▼                             ▼

┌────────────────┐       ┌────────────────────┐       ┌────────────────────┐
│ Move Generator │       │   Search Engine    │       │  Move Execution    │
│   movegen.cpp  │       │  Minimax + ABP     │       │ makeMove/undoMove  │
└───────┬────────┘       └──────────┬─────────┘       └──────────┬─────────┘
        │                           │                            │
        └───────────────────────────┼────────────────────────────┘
                                    ▼
                         ┌──────────────────────┐
                         │  Position Evaluator  │
                         └──────────┬───────────┘
                                    ▼
                         ┌──────────────────────┐
                         │    Best Move Found   │
                         └──────────────────────┘
```

---

# 🌳 Minimax Search Architecture

The engine uses a recursive Minimax search system where:

* The AI tries to maximize score
* The opponent tries to minimize score

Each move is:

1. Simulated
2. Evaluated recursively
3. Undone safely
4. Compared against other candidate moves

---

## Minimax Tree Example

```text
                           AI (MAX)
                     ┌────────┼────────┐
                     ▼        ▼        ▼
                    A         B         C

                 Opponent (MIN)

               ┌──┴──┐   ┌──┴──┐   ┌──┴──┐
               ▼     ▼   ▼     ▼   ▼     ▼
              +3    +5  -2    +1  +4    -6
```

The opponent always chooses the worst outcome for the AI:

* Move A → +3
* Move B → -2
* Move C → -6

Therefore:

```text
Best Move = A
```

---

# ⚡ Alpha-Beta Pruning

To improve performance, the engine implements Alpha-Beta pruning.

This optimization:

* Removes unnecessary search branches
* Reduces computation
* Allows deeper search depth
* Improves engine strength significantly

---

## Alpha-Beta Example

```text
                          MAX
                       /       \
                     MIN       MIN
                    /  \      /   \
                   5    6    2    ❌
```

Once the engine discovers that the right branch cannot outperform the current best move, the remaining nodes are skipped entirely.

---

# 🔄 Move Simulation System

One of the biggest architectural upgrades in the engine was implementing a proper:

```cpp
makeMove()
undoMove()
```

system using move state snapshots.

This allows:

* Deep recursive AI search
* Safe board restoration
* Accurate move simulation
* Reliable handling of:

  * Castling
  * En passant
  * Promotion
  * Captures

Each move stores:

* Captured piece information
* Previous board state
* Castling rights
* En passant state
* Promotion details

This architecture is essential for scalable chess engine development.

---

# 📂 Project Structure

```text
ChessEngine/
│
├── main.cpp
│
├── mainwindow.cpp
├── mainwindow.h
├── mainwindow.ui
│
├── board.cpp
├── board.h
│
├── movegen.cpp
├── movegen.h
│
├── assets/
│   ├── piece_icons/
│   └── sounds/
│
└── CMakeLists.txt
```

---

# ⚙️ Build Requirements

## Required

* CMake ≥ 3.20
* Qt 6
* C++17-compatible compiler

## Tested With

* MinGW-w64
* MSVC
* Qt 6.11

---

# 🛠️ Build Instructions

## Using Qt Creator

1. Open `CMakeLists.txt`
2. Configure Qt kit
3. Build & Run

---

## Command Line (Windows)

```powershell
mkdir build
cd build

cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug ..
cmake --build .
```

---

# ▶️ Running

```powershell
.\build\chessengine.exe
```

---



# 🧪 Planned Improvements

## Engine Improvements

* Zobrist hashing
* Transposition tables
* Iterative deepening
* Move ordering heuristics
* Quiescence search
* Opening book
* Endgame tablebases
* UCI protocol support

---

## Testing Infrastructure

* Perft testing
* Move legality stress tests
* FEN loading
* PGN support
* Search correctness testing

---

# 📚 What This Project Taught Me

This project became much more than a GUI chess application.

It provided hands-on experience with:

* Recursive algorithms
* Search optimization
* State management
* Complex rule validation
* AI architecture
* Performance considerations
* GUI ↔ engine integration

It also helped bridge theoretical CS concepts with real-world engine implementation.

---

# 🤝 Contributing

Pull requests, issue reports, and improvement ideas are welcome.

Areas especially open for contributions:

* Engine optimization
* Search improvements
* Evaluation tuning
* Testing infrastructure
* GUI polish

---

# 📖 References

Concepts inspired by:

* Stockfish
* Chess Programming Wiki
* Classical Minimax research
* Open-source chess engine architectures

---

# 📜 License

No license has been added yet.
