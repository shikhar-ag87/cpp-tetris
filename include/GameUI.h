#ifndef GAMEUI_H
#define GAMEUI_H

#include <wchar.h>
#include <string.h>

void clear_area(int start_x, int start_y, int width, int height);

class Shape {
public:
    int w;
    int h;
    int** data = nullptr;
    int color = 1;
    Shape(int w, int h, std::wstring d);
    Shape(const Shape& other);
    ~Shape();
};

class GameUIComponent {
public:
    int x;
    int y;
    int w;
    int h;
    std::wstring* data = nullptr;
    GameUIComponent(int x, int y, int w, int h);
    ~GameUIComponent();
    void SetPoint(int x, int y, wchar_t v);
    void SetData(std::wstring* data);
    void SetRow(int y, std::wstring data);
    std::wstring GetRow(int y);
    wchar_t GetPoint(int x, int y);
    void DrawBorder();
    void DrawTitle(std::wstring title);
    void DrawTextCentered(std::wstring t);
    void DrawShapeCentered(Shape* sh);
    void Draw();
    void Clear();
};

class GameUI {
public:
    int w;
    int h;
    GameUI(int w, int h);
    ~GameUI();
};

#endif