# ChessEngine

A small C++ Qt6 chess engine and GUI, built with CMake. This repository contains a simple GUI application (`mainwindow.*`) and core engine components (`board.*`, `movegen.*`) used to represent the board and generate moves.

## Features
- Simple playable chess GUI using Qt6
- Board representation and move generation in C++
- CMake-based build suitable for Qt Creator or command-line builds

## Requirements
- CMake (>= 3.20 recommended)
- Qt 6 (6.11 used when this project was built)
- A C++17-capable compiler (MinGW-w64 on Windows or MSVC)

## Build (Windows / Qt Creator)
1. Open the `CMakeLists.txt` project in Qt Creator.
2. Configure the kit (Qt 6, MinGW or MSVC) and run a Debug or Release build.

## Build (Command line)
Run these commands from a PowerShell prompt in the project root:

```powershell
mkdir build
cd build
cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug ..
cmake --build . --config Debug
```

Adjust the generator and flags to match your platform/kit (e.g. use `-G "Ninja"` or MSVC generators).

## Run
- If built with Qt Creator, run from the IDE.
- If built on the command line, the executable will be in `build/` (or the generator-specific output directory). Example:

```powershell
.\build\chessengine.exe
```

## Project Structure
- [main.cpp](main.cpp) — application entry point
- [mainwindow.cpp](mainwindow.cpp), [mainwindow.h](mainwindow.h), [mainwindow.ui](mainwindow.ui) — Qt GUI
- [board.cpp](board.cpp), [board.h](board.h) — board representation
- [movegen.cpp](movegen.cpp), [movegen.h](movegen.h) — move generation logic
- [CMakeLists.txt](CMakeLists.txt) — build configuration

## How to Play
Open the app, click or drag to move pieces on the board. The GUI currently implements basic interaction for playing against the engine or testing move generation (see source for details).

## Contributing
- Fork the repo and open a pull request.
- Provide a brief description of changes and add tests where appropriate.

## Known Issues & TODO
- No formal tests included — adding unit tests for move generation would help.
- Engine strength and search features are basic and can be improved.

## License
No license is specified in this repository. Add a `LICENSE` file to declare terms for reuse.

---