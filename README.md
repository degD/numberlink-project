# YPG-project

A Numberlink-style grid puzzle game and solver written in C for the **BLM1012** course project. The goal is to connect matching pairs of numbers on an $N \times N$ board without overlapping paths or crossing obstacles.

## Project Structure

- **`Proje.c`** — Main implementation including file loading (`readFromFile`), board display (`drawBoard`), manual gameplay with move validation and undo (`gameLoopManual`, `moveUndo`), and automatic route generation and solving (`gamemodeAuto`).
- **`Proje0.c`** — Earlier prototype with an interactive menu, random board generation, manual line drawing, and undo support.
- **`possibleRoutes.c`** — Standalone prototype for recursive route counting and path saving (`count_routes`, `find_save_routes`).

## Build & Run

Compile and run with `gcc`:

```bash
gcc Proje.c -o Proje
./Proje
```

By default, `main()` in `Proje.c` reads a board file from `./TestVerileri/5x5.txt` (`N = 5`) with `row col number` triples per line and runs `gamemodeAuto`.
