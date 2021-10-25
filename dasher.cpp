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
    bool isInAir{false};
    const int j_velocity{25};

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        // Gravity and Ground check
        if (posY + r_height < height) 
        {
            isInAir = true;
            velocity += gravity;
        } 
        else 
        {
            isInAir = false;
            velocity = 0;
            posY = height - r_height;
        }
        // Jump 
        if (IsKeyPressed(KEY_SPACE) && !isInAir) {velocity -=j_velocity;}
        // Location
        posY += velocity;

        BeginDrawing();
        ClearBackground(WHITE);
        DrawRectangle(posX, posY, r_width, r_height, r_color);
        EndDrawing();
    }
    CloseWindow();
}