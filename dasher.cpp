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

bool isOnGround (AnimData data, int height)
{
    return data.pos.y + data.rec.height >= height;
}

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
    const int nebulaCount{8};
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
        if (isOnGround(scarfyData, windowDimension[1])) 
        {
            velocity = 0;
            scarfyData.pos.y = windowDimension[1] - scarfyData.rec.height;
        } 
        else 
        {
            velocity += gravity * dT;
        }

        // Jump 
        if (IsKeyPressed(KEY_SPACE) && isOnGround(scarfyData, windowDimension[1])) {velocity -=j_velocity;}

        // Update scarfy frames
        if (isOnGround(scarfyData, windowDimension[1]))
        {
            scarfyData.runTime += dT;
            if (scarfyData.runTime >= scarfyData.updTime)
            {
                scarfyData.runTime = 0.0f;
                if (scarfyData.frame++ > 5) {scarfyData.frame = 0;}
                scarfyData.rec.x = scarfyData.frame * scarfyData.rec.width;
            }
        }

        // Update nebulae frames
        for (int i = 0; i < nebulaCount; i++)
        {
            nebulae[i].runTime += dT;
            if (nebulae[i].runTime >= nebulae[i].updTime)
            {
                nebulae[i].runTime = 0.0f;
                if (nebulae[i].frame++ > 7) {nebulae[i].frame = 0;}
                nebulae[i].rec.x = nebulae[i].frame * nebulae[i].rec.width;
            } 
        }

        // Location
        for (int i = 0; i < nebulaCount; i++)
        {
            nebulae[i].pos.x += n_velocity * dT;
        }
        scarfyData.pos.y += velocity * dT;

        BeginDrawing();
        ClearBackground(WHITE);
        for (int i = 0; i < nebulaCount; i++)
        {
            DrawTextureRec(neb_tex, nebulae[i].rec, nebulae[i].pos, WHITE);
        }
        DrawTextureRec(scarfy_tex, scarfyData.rec, scarfyData.pos, WHITE);
        EndDrawing();
    }
    UnloadTexture(neb_tex);
    UnloadTexture(scarfy_tex);
    CloseWindow();
}