//cc tankshoot.c  libraylib.a -framework IOKit -framework Cocoa
#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
    SetTargetFPS(60);

    Texture2D tank = LoadTexture("tank.png");
    int tank_y = 400;
    int tank_x = 400;
    int tank_d = 0;
    
    Texture2D enemy = LoadTexture("enemy_tank.png");
    int enemy_x = 400;
    int enemy_y = 100;
    int enemy_d = 180;
    int enemy_life = 1;

    Texture2D bullet = LoadTexture("bullet.png");
    int bullet_x = -100;
    int bullet_y = -100;
    int bullet_d = 0;
    int is_bullet_on_screen = 0;

    while (!WindowShouldClose())
    {
        if (IsKeyDown(KEY_UP)){
            if (tank_d == 0){
                tank_y = tank_y - 1;
            }else
            if (tank_d == 90){
                tank_x = tank_x + 1;
            }else
            if (tank_d == 180){
                tank_y = tank_y + 1;
            }else
            if (tank_d == 270){
                tank_x = tank_x - 1;
            }
        }else
        if (IsKeyDown(KEY_DOWN)){
            if (tank_d == 0){
                tank_y = tank_y + 1;
            }else
            if (tank_d == 90){
                tank_x = tank_x - 1;
            }else
            if (tank_d == 180){
                tank_y = tank_y - 1;
            }else
            if (tank_d == 270){
                tank_x = tank_x + 1;
            }
        }else
        if (IsKeyPressed(KEY_RIGHT)){
            tank_d = tank_d + 90;

            if (tank_d == 90){
                tank_x = tank_x + 50;
            }else
            if (tank_d == 180){
                tank_y = tank_y + 50;
            }else
            if (tank_d == 270){
                tank_x = tank_x - 50;
            }else
            if (tank_d == 360){
                tank_y = tank_y - 50;
                tank_d = 0;
            }
        }else
        if (IsKeyPressed(KEY_LEFT)){
            tank_d = tank_d - 90;
            
            if (tank_d == -90){
                tank_d = 270;
            }

            if (tank_d == 270){
                tank_y = tank_y + 50;
            }else
            if (tank_d == 180){
                tank_x = tank_x + 50;
            }else
            if (tank_d == 90){
                tank_y = tank_y - 50;
            }else
            if (tank_d == 0){
                tank_x = tank_x - 50;
                tank_d = 0;
            }

        }else
        if (IsKeyPressed(KEY_SPACE) && is_bullet_on_screen==0 ){
            bullet_x = tank_x;
            bullet_y = tank_y;
            bullet_d = tank_d;
        }

        if (bullet_d == 0){
            bullet_y = bullet_y - 3;
        }else
        if (bullet_d == 90){
            bullet_x = bullet_x + 3;
        }else
        if (bullet_d == 180){
            bullet_y = bullet_y + 3;
        }else
        if (bullet_d == 270){
            bullet_x = bullet_x - 3;
        }

        if (bullet_x < 0 || bullet_x > screenWidth || bullet_y < 0 || bullet_y > screenHeight){
            is_bullet_on_screen = 0;
        }else{
            is_bullet_on_screen = 1;
        }

        int x_distance = enemy_x - bullet_x;
        int y_distamnce = enemy_y - bullet_y;
        if (x_distance > 0 && x_distance < 50 && y_distamnce > 0 && y_distamnce < 50 ){
            enemy_life = enemy_life - 1;
        }
        if (enemy_d == 0){
            enemy_y = enemy_y - 1;
        }else
            if (enemy_d == 90){
                enemy_x = enemy_x + 1;
            }else
            if (enemy_d == 180){
                enemy_y = enemy_y + 1;
            }else
            if (enemy_d == 270){
                enemy_x = enemy_x - 1;
        }
        BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawTextureEx(tank,(Vector2){tank_x,tank_y},tank_d,1.0,WHITE);
            DrawTextureEx(bullet,(Vector2){bullet_x,bullet_y},bullet_d,1.0,WHITE);
            if (enemy_life > 0){
                DrawTextureEx(enemy,(Vector2){enemy_x,enemy_y},enemy_d,1.0,WHITE);
            }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}