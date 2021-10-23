#include "raylib.h"

int main()
{
    // Window dimensions
    int const width{800}, height{400};
    InitWindow(width, height, "Dapper Dasher");

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);
        EndDrawing();
    }
    CloseWindow();
}