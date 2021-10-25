#include "raylib.h"

int main()
{
    // Window dimensions
    const int width{800}, height{400};
    InitWindow(width, height, "Dapper Dasher");
    // World params
    const int gravity{1};
    int velocity{0};
    // Rectangle params
    const int r_width{50}, r_height{50};
    const Color r_color{BLUE};
    int posX{width/2}, posY{height/2};

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        // Gravity and Ground check
        if (posY + r_height < height) {velocity += gravity;} 
        else 
        {
            velocity = 0;
            posY = height - r_height;
        }
        // Jump 
        if (IsKeyPressed(KEY_SPACE)) {velocity -=10;}
        // Location
        posY += velocity;

        BeginDrawing();
        ClearBackground(WHITE);
        DrawRectangle(posX, posY, r_width, r_height, r_color);
        EndDrawing();
    }
    CloseWindow();
}