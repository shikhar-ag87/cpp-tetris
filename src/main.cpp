#include <iostream>

#include "Tetris.h"

int main() {
    TetrisGame game(20, 30);
    std::cout << "\e[1;32mStarting tetris...\e[0m" << std::endl;
    return game.start();
}