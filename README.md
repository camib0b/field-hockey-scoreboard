
# Field Hockey Scoreboard Simulator 🏑

A console-based field hockey match simulator built in C++20.

## How to Build & Run (macOS / Linux)

```bash
c++ -std=c++20 -Wall -Wextra -pedantic -O2 main.cpp -o hockey_scoreboard
./hockey_scoreboard


# Future Plans

- Real-time match clock using std::chrono and multithreading
- Video analysis integration with OpenCV (player/ball tracking)
- Performance-critical modules in C++ for sports analytics
- Potential GUI version (Qt or ncurses)


# discussed & implemented code improvements
- Clean, professional OOP design
- Perfect naming symmetry with domain-accurate verbs (goalFor..., cardFor..., penaltyCornerFor...)
- Unified private methods (scoreGoalFor, showCardFor, awardPenaltyCornerFor)
- Automatic card names via magic_enum (zero maintenance)
- DRY principle strictly applied
- Const-correctness, move semantics, exhaustive switches
- Highly readable, maintainable, and extensible
- Ready for future features (real-time clock, players, saving matches)
- ignoreLine() after every std::cin >> var_name

## Requirements & Portability

This simulator uses modern C++20 features for clean code and nice output:

- **C++20 compiler** required (GCC 13+, Clang 14+, MSVC 19.29+)
- `<format>` for readable string formatting
- Terminal that supports ANSI escape codes (recommended: macOS Terminal, Linux terminal, Windows Terminal, or Git Bash)
- On old Windows `cmd.exe`, screen clearing uses `cls`

Emoji 🏑 may appear as squares on very old terminals — cosmetic only.

Compile with:
```bash
c++ -std=c++20 -Wall -Wextra -pedantic -O2 main.cpp -o hockey_scoreboard
