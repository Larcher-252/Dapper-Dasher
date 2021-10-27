#include "raylib.h"

// Animation data
struct AnimData
{
    // Sprite params
    Rectangle rec;
    Vector2 pos;
    // Animation params
    int frame;
    int lastFrame;
    float updTime;
    float runTime;
};

bool isOnGround (AnimData data, int height)
{
    return data.pos.y + data.rec.height >= height;
}

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

int main()
{
    // Window dimensions
    int windowDimension[2];
    windowDimension[0] = 512;
    windowDimension[1] = 380;
    InitWindow(windowDimension[0], windowDimension[1], "Dapper Dasher");

    // Background
    Texture2D background = LoadTexture("textures/far-buildings.png");
    Vector2 bg_pos{0.0f, 0.0f};
    const float bg_rotation{0}, bg_scale{2};
    Vector2 bg_pos_2{bg_pos.x + background.width * bg_scale, 0.0f};
    const int bg_velocity = -20;

    Texture2D midground = LoadTexture("textures/back-buildings.png");
    Vector2 mg_pos{0.0f, 0.0f};
    const float mg_rotation{0}, mg_scale{2};
    Vector2 mg_pos_2{mg_pos.x + midground.width * mg_scale, 0.0f};
    const int mg_velocity = -40;

    Texture2D foreground = LoadTexture("textures/foreground.png");
    Vector2 fg_pos{0.0f, 0.0f};
    const float fg_rotation{0}, fg_scale{2};
    Vector2 fg_pos_2{fg_pos.x + foreground.width * fg_scale, 0.0f};
    const int fg_velocity = -80;

    // Hazard sprite params
    Texture2D neb_tex = LoadTexture("textures/12_nebula_spritesheet.png");
    const int n_velocity{-200};
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
    }

    // Scarfy sprite params
    Texture2D scarfy_tex = LoadTexture("textures/scarfy.png");
    int velocity{0};
    const int j_velocity{600};
    AnimData scarfyData{
        {0.0f, 0.0f, scarfy_tex.width/6, scarfy_tex.height},                                    // Rectangle rec
        {windowDimension[0]/2 - scarfy_tex.width/12, windowDimension[1] - scarfy_tex.height},   // Vector2 pos
        0,                                                                                      // int frame
        5,                                                                                      // int lastFrame
        1.0f/12.0f,                                                                             // float updTime
        0.0f                                                                                    // float runTime
    };

    // World params
    const int gravity{1'000};
    float dT;
    float finishLine{nebulae[nebulaCount - 1].pos.x + 100};
    bool collision{false};

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
            scarfyData = animUpdate(scarfyData, dT);
        }

        // Update nebulae frames
        for (int i = 0; i < nebulaCount; i++)
        {
            nebulae[i] = animUpdate(nebulae[i], dT);
        }

        // Location
        bg_pos.x += bg_velocity * dT;
        if (bg_pos.x <= -background.width * bg_scale) {bg_pos.x = 0;}
        bg_pos_2.x = bg_pos.x + background.width * bg_scale;
        mg_pos.x += mg_velocity * dT;
        if (mg_pos.x <= -midground.width * mg_scale) {mg_pos.x = 0;}
        mg_pos_2.x = mg_pos.x + midground.width * mg_scale;
        fg_pos.x += fg_velocity * dT;
        if (fg_pos.x <= -foreground.width * fg_scale) {fg_pos.x = 0;}
        fg_pos_2.x = fg_pos.x + foreground.width * fg_scale;

        for (int i = 0; i < nebulaCount; i++)
        {
            nebulae[i].pos.x += n_velocity * dT;
        }
        scarfyData.pos.y += velocity * dT;
        finishLine += n_velocity * dT;

        for (AnimData nebul : nebulae)
        {
            float pad{50};
            Rectangle nebRec{
                nebul.pos.x + pad,
                nebul.pos.y + pad,
                nebul.rec.width - 2*pad,
                nebul.rec.height - 2*pad
            };
            Rectangle scarfyRec{
                scarfyData.pos.x,
                scarfyData.pos.y,
                scarfyData.rec.width,
                scarfyData.rec.height
            };
            if (CheckCollisionRecs(nebRec, scarfyRec)) {collision = true;}
        }

        BeginDrawing();
        ClearBackground(WHITE);
        if (collision)
        {
            DrawText("You loose!", windowDimension[0] / 2, windowDimension[1] / 2, 40, RED);
        }
        else if (finishLine <= scarfyData.pos.x)
        {
            DrawText("You win!", windowDimension[0] / 2, windowDimension[1] / 2, 40, GREEN);
        }
        else
        {
            DrawTextureEx(background, bg_pos, bg_rotation, bg_scale, WHITE);
            DrawTextureEx(background, bg_pos_2, bg_rotation, bg_scale, WHITE);
            DrawTextureEx(midground, mg_pos, mg_rotation, mg_scale, WHITE);
            DrawTextureEx(midground, mg_pos_2, mg_rotation, mg_scale, WHITE);
            DrawTextureEx(foreground, fg_pos, fg_rotation, fg_scale, WHITE);
            DrawTextureEx(foreground, fg_pos_2, fg_rotation, fg_scale, WHITE);
            for (int i = 0; i < nebulaCount; i++)
            {
                DrawTextureRec(neb_tex, nebulae[i].rec, nebulae[i].pos, WHITE);
            }
            DrawTextureRec(scarfy_tex, scarfyData.rec, scarfyData.pos, WHITE);
        }
        EndDrawing();
    }
    UnloadTexture(neb_tex);
    UnloadTexture(scarfy_tex);
    UnloadTexture(background);
    UnloadTexture(midground);
    UnloadTexture(foreground);
    CloseWindow();
}