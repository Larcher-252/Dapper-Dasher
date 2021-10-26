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
    // First
    Rectangle neb_rec;
    neb_rec.width = neb_tex.height/8;
    neb_rec.height = neb_tex.height/8;
    neb_rec.x = 0;
    neb_rec.y = 0;
    Vector2 neb_pos;
    neb_pos.x = width;
    neb_pos.y = height - neb_rec.height;
    // Animation params
    int nebFrame{0};
    const float nebUpdateTime{1.0f/12.0f};
    float nebRunningTime{0};

    // Second
    Rectangle neb_rec_2;
    neb_rec_2.width = neb_tex.height/8;
    neb_rec_2.height = neb_tex.height/8;
    neb_rec_2.x = 0;
    neb_rec_2.y = 0;
    Vector2 neb_pos_2;
    neb_pos_2.x = width + 300;
    neb_pos_2.y = height - neb_rec.height;
    // Animation params
    int nebFrame_2{0};
    const float nebUpdateTime_2{1.0f/16.0f};
    float nebRunningTime_2{0};

    // Scarfy sprite params
    Texture2D scarfy_tex = LoadTexture("textures/scarfy.png");
    AnimData scarfyData;
    scarfyData.rec.width = scarfy_tex.width/6;
    scarfyData.rec.height = scarfy_tex.height;
    scarfyData.rec.x = 0;
    scarfyData.rec.y = 0;
    scarfyData.pos.x = width/2 - scarfyData.rec.width/2;
    scarfyData.pos.y = height - scarfyData.rec.height;
    scarfyData.frame = 0;
    scarfyData.updTime = 1.0f / 12.0f;
    scarfyData.runTime = 0;

    int velocity{0};
    bool isInAir{false};
    const int j_velocity{600};

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
        neb_pos.x += n_velocity * dT;
        neb_pos_2.x += n_velocity * dT;
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
        nebRunningTime += dT;
        if (nebRunningTime >= nebUpdateTime)
        {
            nebRunningTime = 0.0f;
            if (nebFrame++ > 7) {nebFrame = 0;}
            neb_rec.x = nebFrame * neb_rec.width;
        }

        // Update second nebula frames
        nebRunningTime_2 += dT;
        if (nebRunningTime_2 >= nebUpdateTime_2)
        {
            nebRunningTime_2 = 0.0f;
            if (nebFrame_2++ > 7) {nebFrame_2 = 0;}
            neb_rec_2.x = nebFrame_2 * neb_rec_2.width;
        }

        BeginDrawing();
        ClearBackground(WHITE);
        DrawTextureRec(neb_tex, neb_rec, neb_pos, WHITE);
        DrawTextureRec(neb_tex, neb_rec_2, neb_pos_2, PINK);
        DrawTextureRec(scarfy_tex, scarfyData.rec, scarfyData.pos, WHITE);
        EndDrawing();
    }
    UnloadTexture(neb_tex);
    UnloadTexture(scarfy_tex);
    CloseWindow();
}