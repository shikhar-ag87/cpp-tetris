#include <stdlib.h>
#include <sstream>
#include <string>
#include <ncursesw/ncurses.h>
#include <wchar.h>

#include "GameUI.h"


void clear_area(int start_x, int start_y, int width, int height) {
    std::wstring spaces(width, L' ');
    for (int y = 0; y < height; ++y) {
        mvaddwstr(start_y + y, start_x, spaces.c_str());
    }
}

Shape::Shape(const Shape& other) {
    w = other.w;
    h = other.h;
    color = other.color;

    // Allocate new 2D array for data
    data = new int* [h];
    for (int i = 0; i < h; ++i) {
        data[i] = new int[w];
        for (int j = 0; j < w; ++j) {
            data[i][j] = other.data[i][j];
        }
    }
}

Shape::Shape(int w, int h, std::wstring d) : w(w), h(h) {
    data = new int* [h];
    for (int i = 0; i < h; i++) data[i] = new int[w];

    color = 1;
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            wchar_t ch = d[i * w + j];
            if (ch == L'1') data[i][j] = 1;
            else if (ch == L'0') data[i][j] = 0;
            else data[i][j] = 0;  // default fallback for malformed input
        }
    }
}

Shape::~Shape() {
    for (int i = 0; i < h; i++)
        delete[] data[i];
    delete[] data;
}


GameUIComponent::GameUIComponent(int x, int y, int w, int h) : x(x), y(y), w(w), h(h) {
    data = new std::wstring[h];
    for (int i = 0; i < h; i++) {
        data[i] = L"";
        for (int j = 0; j < w; j++) {
            data[i] += L' ';
        }

    }
}

GameUIComponent::~GameUIComponent() {
    delete[] data;
}

void GameUIComponent::SetPoint(int x, int y, wchar_t v = 0) {
    data[y][x] = v;
}

wchar_t GameUIComponent::GetPoint(int x, int y) {
    return data[y][x];
}

void GameUIComponent::DrawBorder() {

    for (int row = 0; row < h; row++) {
        std::wstring r = GetRow(row);
        if (r.size() == 0) continue;
        if (row == 0 || row == h - 1) {
            for (wchar_t& c : r) c = L'-';
            r[0] = L'+';
            r[r.size() - 1] = L'+';
        }
        else {
            r[0] = L'|';
            r[r.size() - 1] = L'|';
        }
        SetRow(row, r);
    }
}

void GameUIComponent::DrawTitle(std::wstring title) {
    title = L" " + title + L" ";
    int title_len = (int)title.size();

    int available_width = w - 2;
    if (title_len > 0 && title_len <= available_width) {
        int start_x = 1 + (available_width - title_len) / 2;
        for (int i = 0; i < title_len; i++) {
            SetPoint(start_x + i, 0, (wchar_t)title[i]);
        }
    }
}


void GameUIComponent::DrawTextCentered(std::wstring t) {
    if (!data || t.empty()) return;

    int textLength = (int)t.size();
    if (textLength > w) textLength = w; // Truncate if too long

    int offsetX = (w - textLength) / 2;
    int offsetY = h / 2;

    for (int i = 0; i < textLength; ++i) {
        SetPoint(offsetX + i, offsetY, (wchar_t)t[i]);
    }
}
void GameUIComponent::DrawShapeCentered(Shape* sh) {
    int offsetX = (w - sh->w) / 2 - 1;
    int offsetY = (h - sh->h) / 2;

    for (int i = 0; i < h; i++) {
        std::wstring row;
        int row_ems = 0;
        for (int j = 0; j < w - row_ems; j++) {
            // Check if current position is inside the shape's area
            if (i >= offsetY && i < offsetY + sh->h && j >= offsetX && j < offsetX + sh->w) {
                // Get corresponding cell inside the shape
                int shape_i = i - offsetY;
                int shape_j = j - offsetX;

                if (sh->data[shape_i][shape_j] == 1) {
                    wchar_t ch = L'⬜';
                    switch (sh->color) {
                    case 1: ch = L'🟦'; break;
                    case 2: ch = L'🟥'; break;
                    case 3: ch = L'🟧'; break;
                    case 4: ch = L'🟩'; break;
                    case 5: ch = L'🟨'; break;
                    case 6: ch = L'🟪'; break;
                    case 7: ch = L'🟫'; break;
                    }
                    row += ch;
                }
                else {
                    row += L"  ";
                }
                row_ems++;
            }
            else {
                row += L" ";
            }
        }
        SetRow(i, row);
    }
}



void GameUIComponent::SetData(std::wstring* d) {
    for (size_t i = 0; i < h; ++i) {
        data[i] = d[i];
    }
}


void GameUIComponent::SetRow(int y, std::wstring d) {
    if (y<0 || y> h - 1) return;
    data[y] = std::wstring(d);
}

std::wstring GameUIComponent::GetRow(int y) {
    return data[y];
}


void GameUIComponent::Draw() {
    clear_area(x, y, w - 1, h - 1);
    for (int i = 0; i < h; i++) {
        mvaddnwstr(y + i, x, data[i].c_str(), data[i].size());
    }
}

void GameUIComponent::Clear() {
    for (int i = 0; i < h; ++i) {
        data[i] = L"";
        for (int j = 0; j < w; j++) {
            data[i] += L' ';
        }
    }
}



GameUI::GameUI(int w, int h) : w(w), h(h) {}
GameUI::~GameUI() {}
