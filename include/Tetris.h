#ifndef TETRIS_H
#define TETRIS_H

#include <wchar.h>
#include "GameUI.h"


class TetrisShapes {
public:
    Shape* shape_i;
    Shape* shape_j;
    Shape* shape_l;
    Shape* shape_o;
    Shape* shape_z;
    Shape* shape_t;
    Shape* shape_s;
    // Shape* shape_go;

    TetrisShapes() {
        shape_i = new Shape(1, 4, L"1111");
        shape_j = new Shape(2, 3, L"010111");
        shape_l = new Shape(2, 3, L"101011");
        shape_o = new Shape(2, 2, L"1111");
        shape_z = new Shape(3, 2, L"110011");
        shape_t = new Shape(3, 2, L"010111");
        shape_s = new Shape(3, 2, L"011110");
        // shape_go = new Shape(3, 2, L"");
    }
    Shape* getRandom() {
        Shape* shapes[] = {
            shape_i,
            shape_j,
            shape_l,
            shape_o,
            shape_z,
            shape_t,
            shape_s
        };
        return new Shape(*shapes[rand() % (sizeof(shapes) / sizeof(Shape*))]);
    }
};

class GameState {
public:
    GameState(int w, int h);
    ~GameState();
    TetrisShapes tetris_shapes;
    Shape* current_shape;
    Shape* next_shape;
    bool running = true;
    int score = 0;
    int shape_x;
    int shape_y;
    int** data;
    int g_x;
    int g_y;
    int g_w;
    int g_h;
    int GetPoint(int x, int y);
    void SetPoint(int x, int y, int v);

    int getMaxDownMove();

    void PutDown();
    void MoveLeft();
    void MoveRight();
    void NewShape();
    void GameOver();
    void RemoveRow(int y);

    void Update();
    void DisplaceShape(int dx, int dy);
    void ClearShape();
    void MakeShape();
    void Draw(int x, int y);
};

class TetrisGame {
public:
    GameUIComponent* c_game_container;
    GameUIComponent* c_next_piece;
    GameUIComponent* c_score;


    GameUI* game_ui;

    GameState* game_state;

    int score_updated = false;

    int tick = 0;
    int FPS = 5;
    int input;
    bool running = true;

    TetrisGame(int w, int h);
    int start();
    void loop();
    void render();
    void inputThread();
    void handle_resize(int sig);

    void OnKeyPress();
};

#endif