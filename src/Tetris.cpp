#include <chrono>
#include <clocale>
#include <sstream>
#include <thread>
#include <ncursesw/ncurses.h>
#include <wchar.h>
#include <cwchar>
#include <cstdlib>   // for rand()
#include <ctime>     // for time()


#include "GameUI.h"
#include "Tetris.h"

GameState::GameState(int w, int h) : g_w(w), g_h(h) {
    data = new int* [h];
    for (int i = 0; i < h; i++) {
        data[i] = new int[w];
        memset(data[i], 0, w);
    }

}

GameState::~GameState() {
    for (int i = 0; i < g_h; i++) {
        delete[] data[i];
    }
    delete[] data;
    delete current_shape;
    delete next_shape;
}

int GameState::GetPoint(int x, int y) {
    return data[y][x];
}

void GameState::SetPoint(int x, int y, int v) {
    data[y][x] = v;
}


void GameState::Draw(int x, int y) {
    clear_area(x, y, g_w - 1, g_h - 1);
    for (int i = 0; i < g_h; i++) {
        std::wostringstream s;
        for (int j = 0; j < g_w; j++) {
            if (GetPoint(j, i) == 1) s << L'🟦';
            else if (GetPoint(j, i) == 2) s << L'🟥';
            else if (GetPoint(j, i) == 3) s << L'🟧';
            else if (GetPoint(j, i) == 4) s << L'🟩';
            else if (GetPoint(j, i) == 5) s << L'🟨';
            else if (GetPoint(j, i) == 6) s << L'🟪';
            else if (GetPoint(j, i) == 7) s << L'🟫';
            else s << L"  ";
        }
        mvaddnwstr(y + i, x, s.str().c_str(), s.str().size());
    }
}


void GameState::ClearShape() {
    for (int i = 0; i < g_h; i++) {
        for (int j = 0; j < g_w; j++) {
            if (
                (shape_y <= i && i < shape_y + current_shape->h)
                && (shape_x <= j && j < shape_x + current_shape->w)
                ) {
                if (current_shape->data[(i - shape_y)][(j - shape_x)] == 1) SetPoint(j, i, 0);
            }
        }
    }

}

void GameState::MakeShape() {
    for (int i = 0; i < g_h; i++) {
        for (int j = 0; j < g_w; j++) {
            if (
                (shape_y <= i && i < shape_y + current_shape->h)
                && (shape_x <= j && j < shape_x + current_shape->w)
                ) {
                bool isBlock = current_shape->data[(i - shape_y)][(j - shape_x)];
                SetPoint(j, i, isBlock ? current_shape->color : GetPoint(j, i));
            }
        }
    }
}

void GameState::Update() {
    int max_down_dy = getMaxDownMove();
    if (shape_y <= 0 && max_down_dy < current_shape->h) GameOver();
    if (max_down_dy <= 0) {
        NewShape();
        return;
    }
    DisplaceShape(0, -1);
}

void GameState::RemoveRow(int y) {
    score += 10;
    for (int i = y; i > 0; i--) {
        for (int j = 0; j < g_w; j++) {
            SetPoint(j, i, GetPoint(j, i - 1));
        }
    }
}

void GameState::GameOver() {
    running = false;

}

void GameState::NewShape() {
    if (current_shape) {
        delete current_shape;
    }
    current_shape = new Shape(*next_shape);

    if (next_shape) {
        delete next_shape;
    }
    next_shape = tetris_shapes.getRandom();
    next_shape->color = rand() % 7 + 1;

    shape_x = g_w / 2;
    shape_y = 0;
    g_x = 2;
    g_y = 2;

    // Check full rows...
    for (int i = 0; i < g_h; i++) {
        bool isFull = true;
        for (int j = 0; j < g_w; j++) {
            if (GetPoint(j, i) == 0) isFull = false;
        }
        if (isFull) RemoveRow(i);
    }
}



void GameState::PutDown() {
    int max_down_dy = getMaxDownMove();
    DisplaceShape(0, -max_down_dy);
}

void GameState::MoveLeft() {
    DisplaceShape(-1, 0);
}

void GameState::MoveRight() {
    DisplaceShape(1, 0);
}
int GameState::getMaxDownMove() {
    int max_down = g_h;

    for (int j = 0; j < current_shape->w; j++) {
        int bottom_in_shape = -1;

        // Find the bottom-most filled cell in column j of the shape
        for (int i = current_shape->h - 1; i >= 0; i--) {
            if (current_shape->data[i][j] == 1) {
                bottom_in_shape = i;
                break;
            }
        }

        // If no block in this column, skip
        if (bottom_in_shape == -1) continue;

        // Check how far it can go down
        int distance = 0;
        while (true) {
            int board_x = shape_x + j;
            int board_y = shape_y + bottom_in_shape + distance + 1;

            // Out of bounds or collision
            if (board_y >= g_h || GetPoint(board_x, board_y) != 0)
                break;

            distance++;
        }

        // Take the minimum distance across all columns
        if (distance < max_down) {
            max_down = distance;
        }
    }

    return max_down;
}


void GameState::DisplaceShape(int dx, int dy) {

    ClearShape();
    if (shape_y - dy + current_shape->h <= g_h && shape_y - dy >= 0) shape_y -= dy;

    if ((shape_x + dx + current_shape->w <= g_w && shape_x + dx >= 0)
        && !((dx > 0 && GetPoint(shape_x + current_shape->w, shape_y) >= 1)
            || (dx < 0 && GetPoint(shape_x - 1, shape_y) >= 1))) shape_x += dx;
    MakeShape();

}



TetrisGame::TetrisGame(int w, int h) {
    setlocale(LC_ALL, "");
    initscr();
    srand(time(NULL));

    int s_w, s_h;
    getmaxyx(stdscr, s_h, s_w);
    game_state = new GameState(w, h);
    game_ui = new GameUI(s_w, s_h);
}


void TetrisGame::inputThread() {
    while (running) {
        int ch = getch();
        if (ch != ERR) {
            input = ch;
            if (ch == 'q') {
                running = false;
            }
            OnKeyPress();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10)); // prevent CPU spin
    }
}


int TetrisGame::start() {
    noecho();
    cbreak();
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);
    scrollok(stdscr, TRUE);
    curs_set(0);

    const int frameDelay = 1000 / FPS;

    std::thread t(&TetrisGame::inputThread, this);

    game_state->next_shape = game_state->tetris_shapes.getRandom();
    game_state->next_shape->color = rand() % 7 + 1;

    game_state->current_shape = new Shape(*game_state->next_shape);

    game_state->next_shape = game_state->tetris_shapes.getRandom();
    game_state->next_shape->color = rand() % 7 + 1;

    game_state->shape_x = game_state->g_w / 2;
    game_state->shape_y = 0;
    game_state->g_x = 2;
    game_state->g_y = 2;



    c_score = new GameUIComponent(45, 1, 21, 5);
    c_score->DrawBorder();
    c_score->DrawTitle(L"Score");
    c_score->DrawTextCentered(L"Hello");
    c_score->Draw();

    c_next_piece = new GameUIComponent(45, 7, 21, 9);
    c_next_piece->DrawBorder();
    c_next_piece->DrawTitle(L"Next");
    c_next_piece->Draw();

    c_game_container = new GameUIComponent(1, 1, game_state->g_w * 2 + 2, game_state->g_h + 2);
    c_game_container->DrawBorder();
    c_next_piece->DrawShapeCentered(game_state->next_shape);
    c_game_container->DrawTitle(L"Game");
    c_game_container->Draw();

    while (1) {
        auto frameStart = std::chrono::high_resolution_clock::now();

        if (!running) break;

        refresh();
        clear();
        loop();
        render();

        // Frame timing
        auto frameEnd = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = frameEnd - frameStart;
        int sleepTime = frameDelay - static_cast<int>(elapsed.count());
        if (sleepTime > 0)
            std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));

        tick++;
    }

    t.join();
    endwin();
    running = false;
    return 0;
}


void TetrisGame::loop() {
    if (!game_state->running) return;
    game_state->Update();
    score_updated = false;
}

void TetrisGame::render() {
    if (!game_state->running) {
        c_game_container->DrawTextCentered(L"Game Over");
        c_game_container->Draw();
        return;
    }

    c_game_container->Draw();
    c_next_piece->DrawShapeCentered(game_state->next_shape);
    c_next_piece->DrawBorder();
    c_next_piece->DrawTitle(L"Next");
    c_next_piece->Draw();


    game_state->Draw(game_state->g_x, game_state->g_y);
    if (!score_updated) {
        c_score->Clear();
        c_score->DrawBorder();
        c_score->DrawTitle(L"Score");
        c_score->DrawTextCentered(std::to_wstring(game_state->score));
        c_score->Draw();
        score_updated = true;
    }
}

void TetrisGame::OnKeyPress() {
    if (!game_state->running) return;

    int ch = input;
    switch (ch) {
    case KEY_LEFT:
        game_state->MoveLeft();
        break;

    case KEY_RIGHT:
        game_state->MoveRight();
        break;

    case KEY_DOWN:
        game_state->PutDown();
        break;

    default:
        break;
    }
    render();

}