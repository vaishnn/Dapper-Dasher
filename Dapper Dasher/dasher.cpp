#include "raylib.h"
//Struct
struct Data
{
    Rectangle Rec;
    Vector2 pos;
    int frame{};
    float updatetime{};
    float runtime{};
};
int endScreen{0};

bool isonground(Data data, int winheight)
{
    if (data.pos.y >= winheight - data.Rec.height)
    {
        return true;
    }
    else
    {
        return false;
    }
}

Data Upd(Data dat, float dt1, int maxframe)
{
    dat.runtime += dt1;
    if (dat.runtime >= dat.updatetime)
    {
        dat.Rec.x = dat.frame * dat.Rec.width;
        dat.frame++;
        if (dat.frame > maxframe)
        {
            dat.frame = 0;
        }
        dat.runtime = 0.0;
    }
    return dat;
}

int main()
{
    //Dimensions
    int WindowDim[2] = {1920, 1080};

    InitWindow(WindowDim[0], WindowDim[1], "Dapper Dasher");
    ToggleFullscreen();

    //Gravity {acceleration} pixels/s/s
    const int gravity{130};
    const int jumpvelocity{50};
    int velocity{0};
    float velfactor{1.0};

    //Scarfy Variables
    Texture2D scarfy = LoadTexture("textures/scarfy.png");
    Data Scarfydata;
    Scarfydata.Rec.height = scarfy.height;
    Scarfydata.Rec.width = scarfy.width / 6.F;
    Scarfydata.Rec.x = 0;
    Scarfydata.Rec.y = 0;
    Scarfydata.pos.x = WindowDim[0] / 2 - Scarfydata.Rec.width / 2;
    Scarfydata.pos.y = WindowDim[1] - Scarfydata.Rec.height;
    Scarfydata.frame = 0;
    Scarfydata.updatetime = 1.0 / 12.0;
    Scarfydata.runtime = 0.0;

    //Texture of Nebula
    Texture2D Nebula = LoadTexture("textures/neb.png");
    Data NebulaData;
    NebulaData.Rec.height = Nebula.height / 8;
    NebulaData.Rec.width = Nebula.width / 8;
    NebulaData.Rec.x = 0;
    NebulaData.Rec.y = 0;
    NebulaData.pos.x = WindowDim[0];
    NebulaData.pos.y = WindowDim[1] - NebulaData.Rec.height;
    NebulaData.frame = 0;

    //array Nebdate

    //Movement of nebula variables
    int NebulaVel{-600};

    //Is in air
    bool Isinair{};

    //Some Variables
    int count{};

    //Loading Background
    Texture2D back = LoadTexture("textures/far-buildings.png");
    float BgX{0.0};
    float BgX2{}, BgX3{};
    Texture2D back2 = LoadTexture("textures/back-buildings.png");
    Texture2D back3 = LoadTexture("textures/foreground.png");

    //Variable for endscreen
    

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {

        //Frame Time
        float dt = GetFrameTime();

        BeginDrawing();
        ClearBackground(BLACK);

        //Drawing Background
        BgX -= 1000 * dt;
        BgX2 -= 1000 * dt;
        BgX3 -= 1000 * dt;
        Vector2 bgPos1{BgX, 0.0};
        Vector2 bgPos12{BgX + back.width * float(7.4f), 0.0};
        Vector2 bgPos2{BgX2, 0.0};
        Vector2 bgPos22{BgX2 + back2.width * float(7.4f), 0.0};
        Vector2 bgPos3{BgX3, 0.0};
        Vector2 bgPos32{BgX3 + back3.width * float(6.f), 0.0};
        DrawTextureEx(back, bgPos1, 0.0, 7.4f, WHITE);
        DrawTextureEx(back, bgPos12, 0.0, 7.4f, WHITE);
        DrawTextureEx(back2, bgPos2, 0.0, 7.4f, WHITE);
        DrawTextureEx(back2, bgPos22, 0.0, 7.4f, WHITE);
        DrawTextureEx(back3, bgPos3, 0.0, 6.f, WHITE);
        DrawTextureEx(back3, bgPos32, 0.0, 6.f, WHITE);

        if (bgPos1.x <= -back.width * 7.4f)
        {
            BgX = 0;
        }
        if (bgPos2.x <= -back2.width * 7.4f)
        {
            BgX2 = 0;
        }
        if (bgPos3.x <= -back3.width * 6.f)
        {
            BgX3 = 0;
        }

        //Scarfy
        DrawTextureRec(scarfy, Scarfydata.Rec, Scarfydata.pos, WHITE);
        //Is in air
        Scarfydata.pos.y += velocity;
        if (!isonground(Scarfydata, WindowDim[1]))
        {
            Isinair = true;
        }
        if (Isinair)
        {
            velocity += gravity * dt;
        }
        // Position Check
        if (Scarfydata.pos.y >= WindowDim[1] - Scarfydata.Rec.height)
        {
            velocity = 0;
            Scarfydata.pos.y = WindowDim[1] - Scarfydata.Rec.height;
            if (IsKeyPressed(KEY_SPACE))
            {
                velocity = -jumpvelocity;
            }
            Isinair = false;
        }

        //Dealing with motion
        if (!Isinair)
        {
            Scarfydata = Upd(Scarfydata, dt, 5);
        }

        //Moving of Nebula
        DrawTextureRec(Nebula, NebulaData.Rec, NebulaData.pos, WHITE);
        NebulaData.pos.x += NebulaVel * dt * velfactor;

        //Repeatition
        if (NebulaData.pos.x <= 0)
        {
            NebulaData.pos.x = WindowDim[0];
            velfactor += .2;
            count++;
        }
        //Cheching For Collision
        if (count >= 20)
        {
            ClearBackground(BLACK);
            DrawText("YOU WIN", 600, 300, 100, WHITE);
        }
        if (((Scarfydata.pos.x >= NebulaData.pos.x + NebulaData.Rec.width / 4 &&
              Scarfydata.pos.x <= NebulaData.pos.x + NebulaData.Rec.width - NebulaData.Rec.width / 4) ||
             (Scarfydata.pos.x + Scarfydata.Rec.width >= NebulaData.pos.x + NebulaData.Rec.width / 4 &&
              Scarfydata.pos.x + Scarfydata.Rec.width <= NebulaData.pos.x + NebulaData.Rec.width - NebulaData.Rec.width / 4)) &&
            (Scarfydata.pos.y + Scarfydata.Rec.height >= NebulaData.pos.y - NebulaData.Rec.height / 4))
        {
            ClearBackground(BLACK);
            DrawText("YOU LOOSE", 700, 300, 150, WHITE);
            DrawText("Enter to exit", 400, 600, 20, RED);
            endScreen = 1;
            EndDrawing();
            break;
        }

        //Nebula NebulaData.frame change
        NebulaData = Upd(NebulaData, dt, 7);
        EndDrawing();
    }
    UnloadTexture(scarfy);
    UnloadTexture(Nebula);
    UnloadTexture(back);
    while (endScreen == 1)
    {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawText("YOU LOOSE", 700, 300, 150, WHITE);
        DrawText("Enter to exit", 400, 600, 20, RED);
        if (IsKeyDown(KEY_ENTER))
        {
            break;
            
        }
        EndDrawing();
    }
    CloseWindow();
}