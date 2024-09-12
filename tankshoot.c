//cc tankshoot.c  libraylib.a -framework IOKit -framework Cocoa
#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void enemy_appear(int* x,int* y,int *d,int* life ){
    *d = 90 * (rand()%4);

    if (*d == 0){
        *x = (rand() % 800);
        *y = 450;
    }else
    if (*d == 90){
        *x = 0;
        *y = (rand() % 450);
    }else
    if (*d == 180){
        *x = (rand() % 800);
        *y = 0;
    }else
    if (*d == 270){
        *x = 800;
        *y = (rand() % 450);
    }
    *life = 1;

}

int main(void)
{
    int score = 0;
    srand(time(NULL));

    const int screenWidth = 800;
    const int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
    SetTargetFPS(60);

    Texture2D tank = LoadTexture("tank.png");
    int tank_x = 400;
    int tank_y = 225;
    int tank_d = 0;
    
    Texture2D enemy = LoadTexture("enemy_tank.png");
    
    int enemy_x = 0;
    int enemy_y = 0;
    int enemy_d = 0;
    int enemy_life = 0;
    enemy_appear(&enemy_x,&enemy_y,&enemy_d,&enemy_life);

    Texture2D bullet = LoadTexture("bullet.png");
    int bullet_x = -100;
    int bullet_y = -100;
    int bullet_d = 0;
    int is_bullet_on_screen = 0;

    int gameover = 0;

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

            if (tank_d == 360){
                tank_d = 0;
            }
        }else
        if (IsKeyPressed(KEY_LEFT)){
            tank_d = tank_d - 90;
            
            if (tank_d == -90){
                tank_d = 270;
            }
        }
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
        int y_distance = enemy_y - bullet_y;
        if (x_distance > -25 && x_distance < 25 && y_distance > -25 && y_distance < 25 ){
            enemy_life = enemy_life - 1;
        }

        int player_enemy_x = enemy_x - tank_x;
        int player_enemy_y = enemy_y - tank_y;
        if (enemy_life > 0 && player_enemy_x > -25 && player_enemy_x < 25 && player_enemy_y > -25 && player_enemy_y < 25 ){
            gameover = 1;
        }

        if (enemy_life>0){
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

            if (enemy_x < 0 || 800 < enemy_x || enemy_y < 0 || 450 < enemy_y){
                enemy_appear(&enemy_x,&enemy_y,&enemy_d,&enemy_life);
            }
        }else{
            score += 10;
            enemy_appear(&enemy_x,&enemy_y,&enemy_d,&enemy_life);
        }
        BeginDrawing();

            ClearBackground(RAYWHITE);

            //自分の表示
            Vector2 tank_origin = {tank.width / 2.0f,tank.height / 2.0f};
            Rectangle tank_sourceRect = {0.0f,0.0f,(float)tank.width,(float)tank.height};
            Rectangle tank_destRect = {tank_x,tank_y,(float)tank.width,tank.height};
            DrawTexturePro(tank, tank_sourceRect, tank_destRect, tank_origin, tank_d, WHITE);

            //弾の表示
            Vector2 bullet_origin = {bullet.width / 2.0f,bullet.height / 2.0f};
            Rectangle bullet_sourceRect = {0.0f,0.0f,(float)bullet.width,(float)bullet.height};
            Rectangle bullet_destRect = {bullet_x,bullet_y,(float)bullet.width,bullet.height};
            //DrawTextureEx(bullet,(Vector2){bullet_x,bullet_y},bullet_d,1.0,WHITE);
            DrawTexturePro(bullet, bullet_sourceRect, bullet_destRect, bullet_origin, enemy_d, WHITE);
            //DrawTextureEx(tank,(Vector2){tank_x,tank_y},tank_d,1.0,WHITE);
            if (enemy_life > 0){
                //敵の表示
                Vector2 enemy_origin = {enemy.width / 2.0f,enemy.height / 2.0f};
                Rectangle enemy_sourceRect = {0.0f,0.0f,(float)enemy.width,(float)enemy.height};
                Rectangle enemy_destRect = {enemy_x,enemy_y,(float)enemy.width,enemy.height};
                //DrawTextureEx(enemy,(Vector2){enemy_x,enemy_y},enemy_d,1.0,WHITE);
                DrawTexturePro(enemy, enemy_sourceRect, enemy_destRect, enemy_origin, enemy_d, WHITE);
            }
            if (gameover == 1){
                DrawText("GAMEOVER",200,200,70,RED);
            }
            /*
            DrawCircle(tank_x,tank_y,5,RED);
            DrawCircle(enemy_x,enemy_y,5,BLUE);
            DrawCircle(bullet_x,bullet_y,5,YELLOW);
            */
            char scoreText[20];
            sprintf(scoreText,"Score: %d",score);
            DrawText(scoreText,10,10,50,GREEN);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}