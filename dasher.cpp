#include "raylib.h"

// Animation data
struct AnimData
{
    // Sprite params
    Rectangle rec{0.0f, 0.0f, 0.0f, 0.0f};
    Vector2 pos{0.0f, 0.0f};
    // Animation params
    int frame{0};
    int lastFrame{0};
    float updTime{0.0f};
    float runTime{0.0f};
    int velocity{0};
};

// Background data
struct bgData
{
    Vector2 pos1{0.0f, 0.0f};
    Vector2 pos2{0.0f, 0.0f};
    float rotation{0};
    float scale{2};
    int velocity{0};
};

// Check is scarfy on ground
bool isOnGround (AnimData data, int height)
{
    return (data.pos.y + data.rec.height >= height);
}

// Check is scarfy touching nebula
bool checkHazardsCollis (AnimData nebula[], int lastNebula, AnimData scarfy)
{
    const int pad{50};
    for (int i = 0; i < lastNebula; i++)
    {
        Rectangle nebRec{
            nebula[i].pos.x + pad,
            nebula[i].pos.y + pad,
            nebula[i].rec.width - 2*pad,
            nebula[i].rec.height - 2*pad
        };
        Rectangle scarfyRec{
            scarfy.pos.x,
            scarfy.pos.y,
            scarfy.rec.width,
            scarfy.rec.height
        };
        if (CheckCollisionRecs(nebRec, scarfyRec)) {return true;}
    }
    return false;
}

// Update texture frames
AnimData animUpdate (AnimData data, float deltaTime)
{
    data.runTime += deltaTime;
    if (data.runTime >= data.updTime)
    {
        data.runTime = 0.0f;
        if (data.frame++ > data.lastFrame) {data.frame = 0;}
        data.rec.x = data.frame * data.rec.width;
    }
    return data;
}

// Moves background
bgData bgUpdate (bgData data, int width, float deltaTime)
{
    data.pos1.x += data.velocity * deltaTime;
    if (data.pos1.x <= -width * data.scale) {data.pos1.x = 0;}
    data.pos2.x = data.pos1.x + width * data.scale;
    return data;
}

int main()
{
    // Window params
    const int windowDimension[]{512, 380};
    const char Title[] = "Dapper Dasher";
    InitWindow(windowDimension[0], windowDimension[1], Title);

    // Background Data
    bgData bgD[3];
    // Farground
    Texture2D farground = LoadTexture("textures/far-buildings.png");
    bgD[0].pos2.x = bgD[0].pos1.x + farground.width * bgD[0].scale;
    bgD[0].velocity = -20;
    // Midground
    Texture2D midground = LoadTexture("textures/back-buildings.png");
    bgD[1].pos2.x = bgD[1].pos1.x + midground.width * bgD[1].scale;
    bgD[1].velocity = -40;
    // Foreground
    Texture2D foreground = LoadTexture("textures/foreground.png");
    bgD[2].pos2.x = bgD[2].pos1.x + foreground.width * bgD[2].scale;
    bgD[2].velocity = -80;

    // Hazard nebula params
    Texture2D neb_tex = LoadTexture("textures/12_nebula_spritesheet.png");
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
        nebulae[i].lastFrame = 7;
        nebulae[i].updTime = 1.0f/12.0f;
        nebulae[i].runTime = 0;
        nebulae[i].velocity = -200;
    }

    // Scarfy params
    Texture2D scarfy_tex = LoadTexture("textures/scarfy.png");
    AnimData scarfyData{
        {0.0f, 0.0f, scarfy_tex.width/6, scarfy_tex.height},                                    // Rectangle rec
        {windowDimension[0]/2 - scarfy_tex.width/12, windowDimension[1] - scarfy_tex.height},   // Vector2 pos
        0,                                                                                      // int frame
        5,                                                                                      // int lastFrame
        1.0f/12.0f,                                                                             // float updTime
        0.0f,                                                                                   // float runTime
        0                                                                                       // int velocity
    };
    const int j_velocity{600};

    // World params
    const int gravity{1'000};
    float dT;
    float finishLine{nebulae[nebulaCount - 1].pos.x + 100};
    bool loose{false};

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        if (!loose && !(finishLine <= scarfyData.pos.x))
        {
            // Time since last frame
            dT = GetFrameTime();

            // If Scarfy stands on ground
            if (isOnGround(scarfyData, windowDimension[1])) 
            {
                scarfyData.velocity = 0;
                scarfyData.pos.y = windowDimension[1] - scarfyData.rec.height;
            } 
            // If Scarfy is in air
            else {scarfyData.velocity += gravity * dT;}

            // Jump 
            if (IsKeyPressed(KEY_SPACE) && isOnGround(scarfyData, windowDimension[1])) {scarfyData.velocity -=j_velocity;}

            // Update Scarfy frames if he's on ground
            if (isOnGround(scarfyData, windowDimension[1])) {scarfyData = animUpdate(scarfyData, dT);}

            // Update nebulae frames
            for (int i = 0; i < nebulaCount; i++) {nebulae[i] = animUpdate(nebulae[i], dT);}

            // Location
            // Change location for background
            bgD[0] = bgUpdate(bgD[0], farground.width, dT);
            bgD[1] = bgUpdate(bgD[1], midground.width, dT);
            bgD[2] = bgUpdate(bgD[2], foreground.width, dT);
            // Change nebula hazards location
            for (int i = 0; i < nebulaCount; i++) {nebulae[i].pos.x += nebulae[i].velocity * dT;}
            // Change Scarfy location
            scarfyData.pos.y += scarfyData.velocity * dT;
            // Change finish line location
            finishLine += nebulae[nebulaCount - 1].velocity * dT;

            // Check loose event
            loose = checkHazardsCollis(nebulae, nebulaCount, scarfyData);
        }
        
        BeginDrawing();
        ClearBackground(WHITE);

        // Background draw
        DrawTextureEx(farground, bgD[0].pos1, bgD[0].rotation, bgD[0].scale, WHITE);
        DrawTextureEx(farground, bgD[0].pos2, bgD[0].rotation, bgD[0].scale, WHITE);
        // Midground draw
        DrawTextureEx(midground, bgD[1].pos1, bgD[1].rotation, bgD[1].scale, WHITE);
        DrawTextureEx(midground, bgD[1].pos2, bgD[1].rotation, bgD[1].scale, WHITE);
        // Foreground draw
        DrawTextureEx(foreground, bgD[2].pos1, bgD[2].rotation, bgD[2].scale, WHITE);
        DrawTextureEx(foreground, bgD[2].pos2, bgD[2].rotation, bgD[2].scale, WHITE);

        if (loose)
        {DrawText("You loose!", windowDimension[0] / 3, windowDimension[1] / 2 - 20, 40, BLUE);}
        else if (finishLine <= scarfyData.pos.x)
        {DrawText("You win!", windowDimension[0] / 2.6, windowDimension[1] / 2 - 20, 40, GREEN);}
        else
        {
            // Nebula hazards draw
            for (int i = 0; i < nebulaCount; i++)
            {DrawTextureRec(neb_tex, nebulae[i].rec, nebulae[i].pos, WHITE);}
            // Scarfy draw
            DrawTextureRec(scarfy_tex, scarfyData.rec, scarfyData.pos, WHITE);
        }
        EndDrawing();
    }
    UnloadTexture(neb_tex);
    UnloadTexture(scarfy_tex);
    UnloadTexture(farground);
    UnloadTexture(midground);
    UnloadTexture(foreground);
    CloseWindow();
}