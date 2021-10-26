#include "raylib.h"

// Animation data
struct AnimData
{
    // Sprite params
    Rectangle rec;
    Vector2 pos;
    // Animation params
    int frame;
    float updTime;
    float runTime;
};

int main()
{
    // Window dimensions
    const int width{800}, height{800};
    InitWindow(width, height, "Dapper Dasher");
    // World params
    const int gravity{1'000};
    float dT;

    // Hazard sprite params
    Texture2D neb_tex = LoadTexture("textures/12_nebula_spritesheet.png");
    const int n_velocity{-600};
    // First nebula
    AnimData neb1Data{
        {neb_tex.width/8, neb_tex.height/8, 0.0f, 0.0f},    // Rectangle rec
        {width, height - neb_tex.height/8},                 // Vector2 pos
        0.0f,                                               // int frame
        1.0f/12.0f,                                         // float updTime
        0.0f                                                // float runTime
    };
    // Second nebula
    AnimData neb2Data{
        {neb_tex.width/8, neb_tex.height/8, 0.0f, 0.0f},    // Rectangle rec
        {width + 300, height - neb_tex.height/8},           // Vector2 pos
        0.0f,                                               // int frame
        1.0f/16.0f,                                         // float updTime
        0.0f                                                // float runTime
    };

    // Scarfy sprite params
    Texture2D scarfy_tex = LoadTexture("textures/scarfy.png");
    int velocity{0};
    bool isInAir{false};
    const int j_velocity{600};
    AnimData scarfyData{
        {neb_tex.width/6, neb_tex.height, 0.0f, 0.0f},          // Rectangle rec
        {width/2 - neb_tex.width/12, height - neb_tex.height},  // Vector2 pos
        0.0f,                                                   // int frame
        1.0f/12.0f,                                             // float updTime
        0.0f                                                    // float runTime
    };

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        // time since last frame
        dT = GetFrameTime();

        // Gravity and Ground check
        if (scarfyData.pos.y + scarfyData.rec.height < height) 
        {
            isInAir = true;
            velocity += gravity * dT;
        } 
        else 
        {
            isInAir = false;
            velocity = 0;
            scarfyData.pos.y = height - scarfyData.rec.height;
        }

        // Jump 
        if (IsKeyPressed(KEY_SPACE) && !isInAir) {velocity -=j_velocity;}
        
        // Location
        neb1Data.pos.x += n_velocity * dT;
        neb2Data.pos.x += n_velocity * dT;
        scarfyData.pos.y += velocity * dT;

        // Update scarfy frames
        if (!isInAir)
        {
            scarfyData.runTime += dT;
            if (scarfyData.runTime >= scarfyData.updTime)
            {
                scarfyData.runTime = 0.0f;
                if (scarfyData.frame++ > 5) {scarfyData.frame = 0;}
                scarfyData.rec.x = scarfyData.frame * scarfyData.rec.width;
            }
        }

        // Update nebula frames
        neb1Data.runTime += dT;
        if (neb1Data.runTime >= neb1Data.updTime)
        {
            neb1Data.runTime = 0.0f;
            if (neb1Data.frame++ > 7) {neb1Data.frame = 0;}
            neb1Data.rec.x = neb1Data.frame * neb1Data.rec.width;
        }

        // Update second nebula frames
        neb2Data.runTime += dT;
        if (neb2Data.runTime >= neb2Data.updTime)
        {
            neb2Data.runTime = 0.0f;
            if (neb2Data.frame++ > 7) {neb2Data.frame = 0;}
            neb2Data.rec.x = neb2Data.frame * neb2Data.rec.width;
        }

        BeginDrawing();
        ClearBackground(WHITE);
        DrawTextureRec(neb_tex, neb1Data.rec, neb1Data.pos, WHITE);
        DrawTextureRec(neb_tex, neb2Data.rec, neb2Data.pos, PINK);
        DrawTextureRec(scarfy_tex, scarfyData.rec, scarfyData.pos, WHITE);
        EndDrawing();
    }
    UnloadTexture(neb_tex);
    UnloadTexture(scarfy_tex);
    CloseWindow();
}