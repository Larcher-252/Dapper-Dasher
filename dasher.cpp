#include "raylib.h"

int main()
{
    // Window dimensions
    const int width{800}, height{800};
    InitWindow(width, height, "Dapper Dasher");
    // World params
    const int gravity{1'000};
    int velocity{0};
    bool isInAir{false};
    const int j_velocity{600};
    float dT;

    // Scarfy sprite
    Texture2D scarfy_tex = LoadTexture("textures/scarfy.png");
    Rectangle scarfy_rec;
    scarfy_rec.width = scarfy_tex.width/6;
    scarfy_rec.height = scarfy_tex.height;
    scarfy_rec.x = 0;
    scarfy_rec.y = 0;
    Vector2 scarfy_pos;
    scarfy_pos.x = width/2 - scarfy_rec.width/2;
    scarfy_pos.y = height - scarfy_rec.height;

    // Animation params
    int frame{0};
    const float updateTime{1.0f / 12.0f};
    float runningTime{0};

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        // time since last frame
        dT = GetFrameTime();
        runningTime += dT;

        // Gravity and Ground check
        if (scarfy_pos.y + scarfy_rec.height < height) 
        {
            isInAir = true;
            velocity += gravity * dT;
        } 
        else 
        {
            isInAir = false;
            velocity = 0;
            scarfy_pos.y = height - scarfy_rec.height;
        }

        // Jump 
        if (IsKeyPressed(KEY_SPACE) && !isInAir) {velocity -=j_velocity;}
        
        // Location
        scarfy_pos.y += velocity * dT;

        // Update scarfy frames
        if (runningTime >= updateTime)
        {
            runningTime = 0.0f;
            if (frame++ > 5) {frame = 0;}
            scarfy_rec.x = frame * scarfy_rec.width;
        }

        BeginDrawing();
        ClearBackground(WHITE);
        DrawTextureRec(scarfy_tex, scarfy_rec, scarfy_pos, WHITE);
        EndDrawing();
    }
    UnloadTexture(scarfy_tex);
    CloseWindow();
}