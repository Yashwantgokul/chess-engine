# Chess Engine

A small C++ chess engine containing basic board representation and move generation.

## Repository structure

- `board.cpp`, `board.h` — board representation and helpers
- `movegen.cpp`, `movegen.h` — move generation logic
- `main.cpp` — program entry point

## Build

Build with g++ (Linux / MinGW):

```bash
g++ -std=c++17 main.cpp board.cpp movegen.cpp -O2 -o chess
```

On Windows with MSVC, create a project or use the Visual Studio developer command prompt:

```powershell
cl /EHsc /std:c++17 main.cpp board.cpp movegen.cpp /O2 /Fe:chess.exe
```

## Run

From the repository root:

```bash
./chess
```

## Notes

- If the build fails, make sure you have a C++17-capable compiler installed and that the source files exist in the project root.
- Feel free to ask me to expand this README with examples, tests, or contribution guidelines.
