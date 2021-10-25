#include "raylib.h"

int main()
{
    // Window dimensions
    int const width{800}, height{400};
    InitWindow(width, height, "Dapper Dasher");
    int velocity{0}, posX{width/2}, posY{height/2};
    int const r_width{50}, r_height{50};
    Color const r_color{BLUE};
    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_SPACE)) {velocity = -10;} else {velocity = 0;}
        posY += velocity;
        BeginDrawing();
        ClearBackground(WHITE);
        DrawRectangle(posX, posY, r_width, r_height, r_color);
        EndDrawing();
    }
    CloseWindow();
}