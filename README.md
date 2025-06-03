# cpp-tetris

A simple Tetris game implemented in C++ with a terminal-based UI using ncurses.

Made with love, two sleepless nights and 3 coffees trying to get the box rendering fixed.

## Features

-   Classic Tetris gameplay (except rotation for now)
-   Terminal-based user interface
-   Uses [ncursesw](https://invisible-island.net/ncurses/) for rendering

## Project Structure

```
.
├── CMakeLists.txt
├── include/
│   ├── GameUI.h
│   └── Tetris.h
├── src/
│   ├── GameUI.cpp
│   ├── Tetris.cpp
│   └── main.cpp
├── bin/
│   └── tetris_cpp
└── build/
```

-   `include/`: Header files for the game logic and UI
-   `src/`: Source files for the game logic, UI, and main entry point
-   `bin/`: Compiled executable output
-   `build/`: CMake build directory

## Requirements

-   C++17 compatible compiler (e.g., g++)
-   [CMake](https://cmake.org/) >= 3.10
-   [ncursesw](https://invisible-island.net/ncurses/) library

## Building

1. Install dependencies (on Ubuntu/Debian):

    ```sh
    sudo apt-get install build-essential cmake libncursesw5-dev
    ```

2. Clone the repository and build:

    ```sh
    mkdir build
    cd build
    cmake ..
    make
    ```

3. The executable will be in the `bin` directory:

    ```
    bin/tetris_cpp
    ```

## Running

From the project root:

```sh
./bin/tetris_cpp
```

## Controls

-   use left and right arrow key for shifting the piece
-   use down arrow key to put down the piece

## License

MIT License
