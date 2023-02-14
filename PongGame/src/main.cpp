#include "raylib.h"

using namespace std;

Color GroundColor = Color{0, 200, 125, 255};
Color CpuSide = Color{0, 150, 85, 255};
Color MidCircle = Color{0, 220, 155, 150};
Color MenuGrey = Color{0, 120, 55, 255};
Color shadow = Color{12, 12, 12, 50};
Color invis = Color{255, 255, 255, 0};

int player_score = 0;
int cpu_score = 0;

enum Window {MainMenu, GameAI, GameVS, WinScreen};
Window frame = MainMenu;

class Ball 
{
    public:
        float x, y;
        int speed_x, speed_y;
        int radius;

    void Draw() 
    {
        DrawCircle(x, y, radius, invis);
    }

    void Update() 
    {
        x += speed_x;
        y += speed_y;

        if (y - radius <= 0)
        {
            speed_y *= -1;
        }

        if (x + radius >= GetScreenWidth())
        {
            player_score ++;
            ResetBall();
        }
        if (x - radius <= 0)
        {
            cpu_score ++;
            ResetBall();
        }
        if (IsKeyDown(KEY_R))
        {
            player_score = 0;
            cpu_score = 0;
        }
    }

    void ResetBall()
    {
        x = GetScreenWidth()/2;
        y = GetScreenHeight()/2;

        int speed_choices[2] = {-1,1};
        speed_x *= speed_choices[GetRandomValue(0,1)];
        speed_y *= speed_choices[GetRandomValue(0,1)];
    }
};

class Paddle
{
    protected :
        void LimitMovement()
        {
            if (y <= 0)
            {
                y = 0;
            }
            if (y + height >= GetScreenHeight() - 55)
            {
                y = GetScreenHeight() - 55 - height;
            }
        }

    public:
        float x, y;
        float width, height;
        int speed;

    void Draw()
    {
        DrawRectangleRounded(Rectangle{x, y, width, height}, 0.8, 0, invis);
    }

    void Update() 
    {
        if (IsKeyDown(KEY_W))
        {
            y = y - speed;
        }
        if (IsKeyDown(KEY_S))
        {
            y = y + speed;
        }

        LimitMovement();
    }
};

class Playersec
{
    public:
        float x, y;
        float width, height;
        int speed;

    void Draw()
    {
        DrawRectangleRounded(Rectangle{x, y, width, height}, 0.8, 0, invis);
    }

    void Update() 
    {
        if (IsKeyDown(KEY_P))
        {
            y = y - speed;
        }
        if (IsKeyDown(KEY_L))
        {
            y = y + speed;
        }

        if (y <= 0)
        {
            y = 0;
        }
        if (y + height >= GetScreenHeight() - 55)
        {
            y = GetScreenHeight() - 55 - height;
        }
    }
};

class CpuPaddle: public Paddle
{
    public:

    void Update(float ball_y)
    {
        if (y + height / 2 > ball_y)
        {
            y = y - speed;
        }
        if (y + height / 2 <= ball_y)
        {
            y = y + speed;
        }
        
        LimitMovement();
    }
};

class ButtonColl
{
    public:
        float x, y;
        float width, height;
    
    void Draw()
    {
        DrawRectangle(x, y, width, height, invis);
    }
};

class Saperator
{
    public:
        float x, y;
        float width, height;

    void Draw()
    {
        DrawRectangle(x, y, width, height, MenuGrey);
    }
};

Ball ball;
Paddle player;
Playersec Player2;
CpuPaddle cpu;
Saperator sap;
ButtonColl BttnUp;
ButtonColl BttnDown;
ButtonColl start;
ButtonColl stop;
ButtonColl cpuMatch;
ButtonColl pl2;
ButtonColl back;

int main (void) 
{
    const int screen_width = 800;
    const int screen_height = 500;
    InitWindow(screen_width, screen_height, "Pong Game");
    InitAudioDevice();
    Image mainIcon = LoadImage("image/pong.png");
    SetWindowIcon(mainIcon);
    HideCursor();
    SetTargetFPS(60);

    ball.radius = 10;
    ball.x = GetScreenWidth()/2;
    ball.y = GetScreenHeight()/2 - 35;
    ball.speed_x = 0;
    ball.speed_y = 0;

    player.width = 20;
    player.height = 100;
    player.x = 25;
    player.y = screen_height/2 - 40 - (player.height/2);
    player.speed = 0;

    Player2.width = 20;
    Player2.height = 100;
    Player2.x = screen_width - 45;
    Player2.y = screen_height/2 - 40 - (Player2.height/2);
    Player2.speed = 0;

    cpu.width = 20;
    cpu.height = 100;
    cpu.x = screen_width - 45;
    cpu.y = screen_height/2 - 40 - (cpu.height/2);
    cpu.speed = 0;

    sap.width = GetScreenWidth();
    sap.height = 55;
    sap.x = 0;
    sap.y = GetScreenHeight() - 55;

    BttnUp.x = 10;
    BttnUp.y = GetScreenHeight() - 50;
    BttnUp.width = 50;
    BttnUp.height = 50;

    BttnDown.x = 70;
    BttnDown.y = GetScreenHeight() - 50;
    BttnDown.width = 50;
    BttnDown.height = 50;

    start.x = GetScreenWidth() - 120;
    start.y = GetScreenHeight() - 50;
    start.width = 50;
    start.height = 50;

    stop.x = GetScreenWidth() - 60;
    stop.y = GetScreenHeight() - 50;
    stop.width = 50;
    stop.height = 50;

    back.x = 10;
    back.y = GetScreenHeight() - 50;
    back.width = 90;
    back.height = 45;

    cpuMatch.x = GetScreenWidth()/2 - 45;
    cpuMatch.y = GetScreenHeight()/2 + 25;
    cpuMatch.width = 90;
    cpuMatch.height = 45;

    pl2.x = GetScreenWidth()/2 - 45;
    pl2.y = GetScreenHeight()/2 + 75;
    pl2.width = 90;
    pl2.height = 45;

    Image paddle_img = LoadImage("image/paddle.png");
    Texture2D paddle_txture = LoadTextureFromImage(paddle_img);
    UnloadImage(paddle_img);
    Image ball_img = LoadImage("image/ball.png");
    Texture2D ball_txture = LoadTextureFromImage(ball_img);
    UnloadImage(ball_img);
    Image Button_up = LoadImage("image/UpArrow.png");
    Texture2D ButtonUp = LoadTextureFromImage(Button_up);
    UnloadImage(Button_up);
    Image Button_down = LoadImage("image/DownArrow.png");
    Texture2D ButtonDown = LoadTextureFromImage(Button_down);
    UnloadImage(Button_down);
    Image back_img = LoadImage("image/back.png");
    Texture2D back_txture = LoadTextureFromImage(back_img);
    UnloadImage(back_img);
    Image backOnH_img = LoadImage("image/OnHover/backOnH.png");
    Texture2D backOnH_txture = LoadTextureFromImage(backOnH_img);
    UnloadImage(backOnH_img);
    Image start_img = LoadImage("image/start.png");
    Texture2D start_txture = LoadTextureFromImage(start_img);
    UnloadImage(start_img);
    Image startOnH_img = LoadImage("image/OnHover/startOnH.png");
    Texture2D startOnH_txture = LoadTextureFromImage(startOnH_img);
    UnloadImage(startOnH_img);
    Image stop_img = LoadImage("image/stop.png");
    Texture2D stop_txture = LoadTextureFromImage(stop_img);
    UnloadImage(stop_img);
    Image stopOnH_img = LoadImage("image/OnHover/stopOnH.png");
    Texture2D stopOnH_txture = LoadTextureFromImage(stopOnH_img);
    UnloadImage(stopOnH_img);
    Image Cursor_img = LoadImage("image/cursor.png");
    Texture2D Cursor_txture = LoadTextureFromImage(Cursor_img);
    UnloadImage(Cursor_img);
    Image vsAi_img = LoadImage("image/vsai.png");
    Texture2D vsAi_txture = LoadTextureFromImage(vsAi_img);
    UnloadImage(vsAi_img);
    Image vsAiOnH_img = LoadImage("image/OnHover/vsaiOnH.png");
    Texture2D vsAiOnH_txture = LoadTextureFromImage(vsAiOnH_img);
    UnloadImage(vsAiOnH_img);
    Image vspOnH_img = LoadImage("image/OnHover/vspOnH.png");
    Texture2D vspOnH_txture = LoadTextureFromImage(vspOnH_img);
    UnloadImage(vspOnH_img);
    Image pl2_img = LoadImage("image/vsp.png");
    Texture2D pl2_txture = LoadTextureFromImage(pl2_img);
    UnloadImage(pl2_img);
    Image Pong = LoadImage("image/PongGame.png");
    Texture2D Pong_txture = LoadTextureFromImage(Pong);
    UnloadImage(Pong);

    Sound Bttnsfx = LoadSound("sfx/Button.wav");
    Sound score = LoadSound("sfx/scored.wav");

    Vector2 mousePoint = {0.0f, 0.0f};

    while (WindowShouldClose() == false) 
    {
        BeginDrawing();

        mousePoint = GetMousePosition();
        
        switch (frame)
        {
        case MainMenu:
            ClearBackground(GroundColor);
            DrawTexture(vsAi_txture, cpuMatch.x, cpuMatch.y, WHITE);
            DrawTexture(pl2_txture, pl2.x, pl2.y, WHITE);
            DrawTexture(Pong_txture, 200, 40, WHITE);
            DrawTexture(paddle_txture, 90, 90, WHITE);
            DrawTexture(paddle_txture, GetScreenWidth() - 110, 90, WHITE);
            DrawText("Have you read the 'Readme.txt' ?", 2, GetScreenHeight() - 20, 18, MenuGrey);
            if (CheckCollisionPointRec(Vector2{mousePoint}, Rectangle{cpuMatch.x, cpuMatch.y, cpuMatch.width, cpuMatch.height}))
            {
                DrawTexture(vsAiOnH_txture, cpuMatch.x, cpuMatch.y, WHITE);
                DrawTexture(ball_txture, cpuMatch.x - 25, cpuMatch.y + 13, WHITE);
                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                {
                    frame = GameAI;
                    PlaySound(Bttnsfx);
                }
            }
            if (CheckCollisionPointRec(Vector2{mousePoint}, Rectangle{pl2.x, pl2.y, pl2.width, pl2.height}))
            {
                DrawTexture(vspOnH_txture, pl2.x, pl2.y, WHITE);
                DrawTexture(ball_txture, pl2.x - 25, pl2.y + 13, WHITE);
                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                {
                    frame = GameVS;
                    PlaySound(Bttnsfx);
                }
            }
            break;

        case WinScreen:
            ClearBackground(GroundColor);
            DrawTexture(back_txture, back.x, back.y, WHITE);
            if (cpu_score == 10)
            {
                DrawText("Right Player Win!!!\n          >", 100, GetScreenHeight()/2 - 20, 60, MenuGrey);
            }
            if (player_score == 10)
            {
                DrawText("Left Player Win!!!\n        <", 100, GetScreenHeight()/2 - 20, 60, MenuGrey);
            }
            if (CheckCollisionPointRec(Vector2{mousePoint}, Rectangle{back.x, back.y, back.width, back.height}))
            {
                DrawTexture(backOnH_txture, back.x, back.y, WHITE);
                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                {
                    ball.speed_x = 0;
                    ball.speed_y = 0;
                    ball.x = GetScreenWidth()/2;
                    ball.y = GetScreenHeight()/2 - 35;
                    cpu.speed = 0;
                    cpu.y = screen_height/2 - 40 - (cpu.height/2);
                    player.speed = 0;
                    player.y = screen_height/2 - 40 - (player.height/2);
                    player_score = 0;
                    cpu_score = 0;
                    PlaySound(Bttnsfx);
                    frame = MainMenu;
                    
                }
            }
            break;

        case GameAI:
            ball.Update();
            player.Update();
            cpu.Update(ball.y);
        
            if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{player.x, player.y, player.width, player.height}))
            {
                ball.speed_x *= -1;
            }
            if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{cpu.x, cpu.y, cpu.width, cpu.height}))
            {
                ball.speed_x *= -1;
            }
            if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{sap.x, sap.y, sap.width, sap.height}))
            {
                ball.speed_y *= -1;
            }
            if (cpu_score == 10)
            {
                frame = WinScreen;
            }
            if (player_score == 10)
            {
                frame = WinScreen;
            }
            if (ball.x + ball.radius >= GetScreenWidth() - 2)
            {
                PlaySound(score);
            }
            if (ball.x - ball.radius <= 2)
            {
                PlaySound(score);
            }

            ClearBackground(GroundColor);
            DrawLine(GetScreenWidth()/2, 0, GetScreenWidth()/2, GetScreenHeight() - 55, WHITE);
            DrawRectangle(GetScreenWidth()/2, 0, GetScreenWidth()/2, GetScreenHeight() - 55, CpuSide);
            DrawCircle(ball.x - 5, ball.y + 5, ball.radius, shadow);
            DrawRectangleRounded(Rectangle{player.x - 7, player.y + 7, player.width, player.height}, 0.8, 0, shadow);
            DrawRectangleRounded(Rectangle{cpu.x - 7, cpu.y + 7, cpu.width, cpu.height}, 0.8, 0, shadow);
            sap.Draw();
            DrawText("By\nMiigel", 5, 5, 20, CpuSide);
            DrawText("P1 ", 110, GetScreenHeight() - 45, 20, WHITE);
            DrawLine(105 ,GetScreenHeight() - 25, 200, GetScreenHeight() - 25, WHITE);
            DrawText("AI ", 110, GetScreenHeight() - 25, 20, WHITE);
            DrawText(TextFormat("%i", player_score), 150, GetScreenHeight() - 45, 20, WHITE);
            DrawText(TextFormat("%i", cpu_score), 150, GetScreenHeight() - 25, 20, WHITE);
            DrawTexture(paddle_txture, player.x, player.y, WHITE);
            DrawTexture(paddle_txture, cpu.x, cpu.y, WHITE);
            DrawTexture(ball_txture, ball.x - ball.radius, ball.y - ball.radius, WHITE);
            DrawTexture(start_txture, start.x, start.y, WHITE);
            DrawTexture(stop_txture, stop.x, stop.y, WHITE);
            DrawTexture(back_txture, back.x, back.y, WHITE);
            if (CheckCollisionPointRec(Vector2{mousePoint}, Rectangle{back.x, back.y, back.width, back.height}))
            {
                DrawTexture(backOnH_txture, back.x, back.y, WHITE);
                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                {
                    ball.speed_x = 0;
                    ball.speed_y = 0;
                    ball.x = GetScreenWidth()/2;
                    ball.y = GetScreenHeight()/2 - 35;
                    cpu.speed = 0;
                    cpu.y = screen_height/2 - 40 - (cpu.height/2);
                    player.speed = 0;
                    player.y = screen_height/2 - 40 - (player.height/2);
                    player_score = 0;
                    cpu_score = 0;
                    PlaySound(Bttnsfx);
                    frame = MainMenu;
                }
            }
            if (CheckCollisionPointRec(Vector2{mousePoint}, Rectangle{stop.x, stop.y, stop.width, stop.height}))
            {
                DrawTexture(stopOnH_txture, stop.x, stop.y, WHITE);
                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                {
                    ball.speed_x = 0;
                    ball.speed_y = 0;
                    ball.x = GetScreenWidth()/2;
                    ball.y = GetScreenHeight()/2 - 35;
                    cpu.speed = 0;
                    cpu.y = screen_height/2 - 40 - (cpu.height/2);
                    player.speed = 0;
                    player.y = screen_height/2 - 40 - (player.height/2);
                    PlaySound(Bttnsfx);
                }
            }
            if (CheckCollisionPointRec(Vector2{mousePoint}, Rectangle{start.x, start.y, start.width, start.height}))
            {
                DrawTexture(startOnH_txture, start.x, start.y, WHITE);
                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                {
                    ball.speed_x = 6;
                    ball.speed_y = 6;
                    ball.x = GetScreenWidth()/2;
                    ball.y = GetScreenHeight()/2 - 35;
                    cpu.speed = 4;
                    cpu.y = screen_height/2 - 40 - (cpu.height/2);
                    player.speed = 5;
                    player.y = screen_height/2 - 40 - (player.height/2);
                    PlaySound(Bttnsfx);
                }
            }
            break;

        case GameVS:
            ball.Update();
            player.Update();
            Player2.Update();
        
            if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{player.x, player.y, player.width, player.height}))
            {
                ball.speed_x *= -1;
            }
            if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{Player2.x, Player2.y, Player2.width, Player2.height}))
            {
                ball.speed_x *= -1;
            }
            if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{sap.x, sap.y, sap.width, sap.height}))
            {
                ball.speed_y *= -1;
            }
            if (cpu_score == 10)
            {
                frame = WinScreen;
            }
            if (player_score == 10)
            {
                frame = WinScreen;
            }

            ClearBackground(GroundColor);
            DrawLine(GetScreenWidth()/2, 0, GetScreenWidth()/2, GetScreenHeight() - 55, WHITE);
            DrawRectangle(GetScreenWidth()/2, 0, GetScreenWidth()/2, GetScreenHeight() - 55, CpuSide);
            DrawCircle(ball.x - 5, ball.y + 5, ball.radius, shadow);
            DrawRectangleRounded(Rectangle{player.x - 7, player.y + 7, player.width, player.height}, 0.8, 0, shadow);
            DrawRectangleRounded(Rectangle{Player2.x - 7, Player2.y + 7, Player2.width, Player2.height}, 0.8, 0, shadow);
            sap.Draw();
            DrawText("By\nMiigel", 5, 5, 20, CpuSide);
            DrawText("P1 ", 110, GetScreenHeight() - 45, 20, WHITE);
            DrawLine(105 ,GetScreenHeight() - 25, 200, GetScreenHeight() - 25, WHITE);
            DrawText("P2 ", 110, GetScreenHeight() - 25, 20, WHITE);
            DrawText(TextFormat("%i", player_score), 150, GetScreenHeight() - 45, 20, WHITE);
            DrawText(TextFormat("%i", cpu_score), 150, GetScreenHeight() - 25, 20, WHITE);
            DrawTexture(paddle_txture, player.x, player.y, WHITE);
            DrawTexture(paddle_txture, Player2.x, Player2.y, WHITE);
            DrawTexture(ball_txture, ball.x - ball.radius, ball.y - ball.radius, WHITE);
            DrawTexture(start_txture, start.x, start.y, WHITE);
            DrawTexture(stop_txture, stop.x, stop.y, WHITE);
            DrawTexture(back_txture, back.x, back.y, WHITE);
            if (CheckCollisionPointRec(Vector2{mousePoint}, Rectangle{back.x, back.y, back.width, back.height}))
            {
                DrawTexture(backOnH_txture, back.x, back.y, WHITE);
                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                {
                    ball.speed_x = 0;
                    ball.speed_y = 0;
                    ball.x = GetScreenWidth()/2;
                    ball.y = GetScreenHeight()/2 - 35;
                    cpu.speed = 0;
                    cpu.y = screen_height/2 - 40 - (cpu.height/2);
                    player.speed = 0;
                    player.y = screen_height/2 - 40 - (player.height/2);
                    player_score = 0;
                    cpu_score = 0;
                    PlaySound(Bttnsfx);
                    frame = MainMenu;
                }
            }
            if (CheckCollisionPointRec(Vector2{mousePoint}, Rectangle{stop.x, stop.y, stop.width, stop.height}))
            {
                DrawTexture(stopOnH_txture, stop.x, stop.y, WHITE);
                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                {
                    ball.speed_x = 0;
                    ball.speed_y = 0;
                    ball.x = GetScreenWidth()/2;
                    ball.y = GetScreenHeight()/2 - 35;
                    player.speed = 0;
                    player.y = screen_height/2 - 40 - (player.height/2);
                    Player2.speed = 0;
                    Player2.y = screen_height/2 - 40 - (Player2.height/2);
                    PlaySound(Bttnsfx);
                }
            }
            if (CheckCollisionPointRec(Vector2{mousePoint}, Rectangle{start.x, start.y, start.width, start.height}))
            {
                DrawTexture(startOnH_txture, start.x, start.y, WHITE);
                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                {
                    ball.speed_x = 6;
                    ball.speed_y = 6;
                    ball.x = GetScreenWidth()/2;
                    ball.y = GetScreenHeight()/2 - 35;
                    player.speed = 5;
                    player.y = screen_height/2 - 40 - (player.height/2);
                    Player2.speed = 5;
                    Player2.y = screen_height/2 - 40 - (Player2.height/2);
                    PlaySound(Bttnsfx);
                }
            }
            break;
        }
        DrawTexture(Cursor_txture, GetMouseX() - 1, GetMouseY() - 1, WHITE);

        EndDrawing();
    }

    UnloadTexture(start_txture);
    UnloadTexture(startOnH_txture);
    UnloadTexture(stop_txture);
    UnloadTexture(stopOnH_txture);
    UnloadTexture(paddle_txture);
    UnloadTexture(ball_txture);
    UnloadTexture(ButtonUp);
    UnloadTexture(ButtonDown);
    UnloadSound(Bttnsfx);
    UnloadSound(score);
    UnloadTexture(Cursor_txture);
    UnloadTexture(vsAi_txture);
    UnloadTexture(vsAiOnH_txture);
    UnloadTexture(vspOnH_txture);
    UnloadTexture(pl2_txture);
    UnloadTexture(back_txture);
    UnloadTexture(backOnH_txture);
    UnloadTexture(Pong_txture);
    CloseWindow();
    return 0;
}
