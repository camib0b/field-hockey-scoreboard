
# Field Hockey Scoreboard Simulator 🏑

A console-based field hockey match simulator built in **modern C++20**.

This project combines my passion for field hockey with professional software engineering practices. It's my first public C++ portfolio piece while preparing for a Master's in Europe.

![Field Hockey Action](https://media.istockphoto.com/id/1498646709/photo/close-up-image-of-attack-in-field-hockey-game.jpg?s=612x612&w=0&k=20&c=0IF5RhdWHUvZnORrh6NO_c0Zd8uHxij9SDYZT3m1n-w=)

## Features
- Clean object-oriented design with dedicated classes: `Team`, `MatchEvent`, and `HockeyMatch`
- Full match simulation: goals, green/yellow/red cards, penalty corners, quarter progression
- Chronological event logging with formatted timeline
- Modern C++ best practices:
  - Strong encapsulation and const-correctness
  - Move semantics for efficiency
  - Scoped enums (`enum class`)
  - Defensive programming (exhaustive switch handling)
  - Pre-increment and clear naming conventions

## Demo Screenshot

![Console Demo](https://learn.microsoft.com/en-us/cpp/build/media/vs2019-hello-world-code.png?view=msvc-170)

*(Sample console output – your actual match will look similar with scoreboard and events)*

## How to Build & Run (macOS / Linux)

```bash
c++ -std=c++20 -Wall -Wextra -pedantic -O2 main.cpp -o hockey_scoreboard
./hockey_scoreboard


# Future Plans

- Real-time match clock using std::chrono and multithreading
- Video analysis integration with OpenCV (player/ball tracking)
- Performance-critical modules in C++ for sports analytics
- Potential GUI version (Qt or ncurses)
