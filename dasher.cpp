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
    int windowDimension[2];
    windowDimension[0] = 512;
    windowDimension[1] = 380;
    InitWindow(windowDimension[0], windowDimension[1], "Dapper Dasher");
    // World params
    const int gravity{1'000};
    float dT;

    // Hazard sprite params
    Texture2D neb_tex = LoadTexture("textures/12_nebula_spritesheet.png");
    const int n_velocity{-400};
    const int nebulaCount{3};
    AnimData nebulae[nebulaCount];
    for (int i = 0; i < nebulaCount; i++)
    {
        // Rectangle rec
        nebulae[i].rec.x = 0.0f;
        nebulae[i].rec.y = 0.0f;
        nebulae[i].rec.width = neb_tex.width/8;
        nebulae[i].rec.height = neb_tex.height/8;
        // Vector2 pos
        nebulae[i].pos.x = windowDimension[0] + 300 * i;
        nebulae[i].pos.y = windowDimension[1] - neb_tex.height/8;
        // Animation params
        nebulae[i].frame = 0;
        nebulae[i].updTime = 1.0f/12.0f;
        nebulae[i].runTime = 0;
    }

    // Scarfy sprite params
    Texture2D scarfy_tex = LoadTexture("textures/scarfy.png");
    int velocity{0};
    bool isInAir{false};
    const int j_velocity{600};
    AnimData scarfyData{
        {0.0f, 0.0f, scarfy_tex.width/6, scarfy_tex.height},                                    // Rectangle rec
        {windowDimension[0]/2 - scarfy_tex.width/12, windowDimension[1] - scarfy_tex.height},   // Vector2 pos
        0,                                                                                      // int frame
        1.0f/12.0f,                                                                             // float updTime
        0.0f                                                                                    // float runTime
    };

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        // time since last frame
        dT = GetFrameTime();

        // Gravity and Ground check
        if (scarfyData.pos.y + scarfyData.rec.height < windowDimension[1]) 
        {
            isInAir = true;
            velocity += gravity * dT;
        } 
        else 
        {
            isInAir = false;
            velocity = 0;
            scarfyData.pos.y = windowDimension[1] - scarfyData.rec.height;
        }

        // Jump 
        if (IsKeyPressed(KEY_SPACE) && !isInAir) {velocity -=j_velocity;}
        
        // Location
        nebulae[0].pos.x += n_velocity * dT;
        nebulae[1].pos.x += n_velocity * dT;
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
        nebulae[0].runTime += dT;
        if (nebulae[0].runTime >= nebulae[0].updTime)
        {
            nebulae[0].runTime = 0.0f;
            if (nebulae[0].frame++ > 7) {nebulae[0].frame = 0;}
            nebulae[0].rec.x = nebulae[0].frame * nebulae[0].rec.width;
        }

        // Update second nebula frames
        nebulae[1].runTime += dT;
        if (nebulae[1].runTime >= nebulae[1].updTime)
        {
            nebulae[1].runTime = 0.0f;
            if (nebulae[1].frame++ > 7) {nebulae[1].frame = 0;}
            nebulae[1].rec.x = nebulae[1].frame * nebulae[1].rec.width;
        }

        BeginDrawing();
        ClearBackground(WHITE);
        DrawTextureRec(neb_tex, nebulae[0].rec, nebulae[0].pos, WHITE);
        DrawTextureRec(neb_tex, nebulae[1].rec, nebulae[1].pos, PINK);
        DrawTextureRec(scarfy_tex, scarfyData.rec, scarfyData.pos, WHITE);
        EndDrawing();
    }
    UnloadTexture(neb_tex);
    UnloadTexture(scarfy_tex);
    CloseWindow();
}