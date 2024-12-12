//cc tankshoot.c  libraylib.a -framework IOKit -framework Cocoa
#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct Bullet{
    int s;
    int x;
    int y;
    int d;
};

void Bullet_init(struct Bullet* this){
    this->s = 0;
    this->x = -100;
    this->y = -100;
    this->d = 0;
}

void Bullet_shot(struct Bullet* this, int tank_x, int tank_y, int tank_d, int tank_s){
    this->s = tank_s + 1;
    this->x = tank_x;
    this->y = tank_y;
    this->d = tank_d;
}

void Bullet_draw(struct Bullet* this, Texture2D* bullet){
    if(this->s){
        Vector2 bullet_origin = {bullet->width / 2.0f,bullet->height / 2.0f};
        Rectangle bullet_sourceRect = {0.0f,0.0f,(float)bullet->width,(float)bullet->height};
        Rectangle bullet_destRect = {this->x,this->y,(float)bullet->width,bullet->height};
        DrawTexturePro(*bullet, bullet_sourceRect, bullet_destRect, bullet_origin, this->d, WHITE);
    }
}
// Bullet/



struct Landmine{
    int x;
    int y;
    int active;
};

void Landmine_init(struct Landmine* this){
    this->x = -100;
    this->y = -100;
    this->active = 0;
}

void Landmine_col(struct Landmine* this, int* enemy_x, int* enemy_y,int* enemy_life){
    bool got_landmine = CheckCollisionCircles((Vector2){*enemy_x, *enemy_y}, 25, (Vector2){this->x, this->y}, 15);
    if (got_landmine){
        *enemy_life = 0;
        Landmine_init(this);
    }
}

int Landmine_col_tank(struct Landmine* this, int* tank_x, int* tank_y){
    bool got_landmine = CheckCollisionCircles((Vector2){*tank_x, *tank_y}, 25, (Vector2){this->x, this->y}, 15);
    if (got_landmine){
        if(this->active){
            Landmine_init(this);
            return 1;
        }
    }else{
        if(!this->active && this->x != -100 && this->y -100){
            this->active = 1;
        }
    }

    return 0;
}



// for Bullets[]
struct Bullet* takeUnusedBullet(struct Bullet bullets[], int count){
    while(0 <= --count){
        if(bullets[count].s){
            // none
        }else{
            return &bullets[count];
        }
    }
    return NULL;
}
// Bullets/



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

int main2(void)
{
    int ret = 0;

    int score = 0;

    const int screenWidth = 800;
    const int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
    SetTargetFPS(60);

    Texture2D tank = LoadTexture("res/tank.png");
    int tank_x = 400;
    int tank_y = 225;
    int tank_d = 0;
    int tank_s = 1;
    
    Texture2D enemy = LoadTexture("res/enemy_tank.png");
    
    int enemy_x = 0;
    int enemy_y = 0;
    int enemy_d = 0;
    int enemy_spd = 5;
    int enemy_life = 0;
    enemy_appear(&enemy_x,&enemy_y,&enemy_d,&enemy_life);

    Texture2D bullet = LoadTexture("res/bullet.png");
    int bullet_x = -100;
    int bullet_y = -100;
    int bullet_d = 0;
    int is_bullet_on_screen = 0;

    #define BULL_N 3
    struct Bullet bullets[BULL_N];

    int j = BULL_N;
    while(0 <= --j){
        Bullet_init(&bullets[j]);
    }

    Texture2D landmine = LoadTexture("res/landmine.png");
    struct Landmine lmine;
    Landmine_init(&lmine);

    Sound baaan = LoadSound("res/baaan.wav");

    int gameover = 0;

    int gameclear = 0;
    int clearscore = 100;

    int mouse_x;//マウスのx座標
    int mouse_y;//マウスのy座標
    int distance_x;//マウスと戦車の距離x
    int distance_y;//マウスと戦車の距離y

    int clear_time;//タイム

    int hera_time = GetTime();//スコアを減らすタイム

    Texture2D enemy_bullet = LoadTexture("res/enemy_bullet.png");
    int enemy_bullet_x = -100;
    int enemy_bullet_y = -100;
    int enemy_bullet_d = 0;
    int enemy_bullet_spd = 0;
    int enemy_bullet_t = GetTime();

    int item_x = -100;
    int item_y = -100;

    Sound item_get = LoadSound("res/item_get.mp3");
    Sound gameclear_sound = LoadSound("res/gameclear.mp3");
    Sound gameover_sound = LoadSound("res/gameover.mp3");
    Sound gekiha = LoadSound("res/explosion.mp3");

    Texture2D background = LoadTexture("res/background.png");

    while (!WindowShouldClose())
    {
        if (gameover == 0 && gameclear == 0){
            mouse_x = GetTouchX();
            mouse_y = GetTouchY();
            distance_x = mouse_x - tank_x;//マウスと戦車との距離X
            distance_y = mouse_y - tank_y;//マウスと戦車との距離Y
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){//タップされたとき
                if (abs(distance_x) < 25 && abs(distance_y) < 25  && is_bullet_on_screen==0){//戦車がクリックされ、弾発射中でないなら発射
                    bullet_x = tank_x;
                    bullet_y = tank_y;
                    bullet_d = tank_d;
                }
            }
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && (abs(distance_x) > 25 || abs(distance_y) > 25) ){//画面が押され、戦車がクリックされていないとき
                if (abs(distance_x) > abs(distance_y) ){//横移動のとき
                    if (distance_x > 0){//右移動
                        tank_d = 90;
                        tank_x = tank_x + tank_s;
                    }else{//左移動
                        tank_d = 270;
                        tank_x = tank_x - tank_s;
                    }
                }else
                if (abs(distance_y) > abs(distance_x) ){//縦移動のとき
                    if (distance_y > 0){//下移動
                        tank_d = 180;
                        tank_y = tank_y + tank_s;
                    }else{//上移動
                        tank_d = 0;
                        tank_y = tank_y - tank_s;
                    }
                }
            }
            if (IsKeyDown(KEY_UP)){
                if (tank_d == 0){
                    tank_y = tank_y - tank_s;
                }else
                if (tank_d == 90){
                    tank_x = tank_x + tank_s;
                }else
                if (tank_d == 180){
                    tank_y = tank_y + tank_s;
                }else
                if (tank_d == 270){
                    tank_x = tank_x - tank_s;
                }
            }else
            if (IsKeyDown(KEY_DOWN)){
                if (tank_d == 0){
                    tank_y = tank_y + tank_s;
                }else
                if (tank_d == 90){
                    tank_x = tank_x - tank_s;
                }else
                if (tank_d == 180){
                    tank_y = tank_y - tank_s;
                }else
                if (tank_d == 270){
                    tank_x = tank_x + tank_s;
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
                if(IsKeyDown(KEY_LEFT_SHIFT) && lmine.x == -100 && lmine.y == -100){
                    lmine.x = tank_x;
                    lmine.y = tank_y;
                }else{
                    struct Bullet* pbullet = takeUnusedBullet(bullets, BULL_N);
                    if(pbullet){
                        Bullet_shot(pbullet, tank_x, tank_y, tank_d, tank_s);
                        PlaySound(baaan);
                    }else{
                        // none
                    }
                    
                }

            }
            if(IsKeyPressed(KEY_F)){
                ToggleFullscreen();
            }

            // enemy_think
            {
                if(5 < GetTime() - enemy_bullet_t){
                    enemy_bullet_x = enemy_x;
                    enemy_bullet_y = enemy_y;
                    enemy_bullet_d = enemy_d;
                    enemy_bullet_spd = enemy_spd + 3;
                    enemy_bullet_t = GetTime();
                }

                if(enemy_bullet_x != -100 && enemy_bullet_y != -100){
                    if(enemy_bullet_d == 0){
                        enemy_bullet_y = enemy_bullet_y - enemy_bullet_spd;
                    }else
                    if(enemy_bullet_d == 90){
                        enemy_bullet_x = enemy_bullet_x + enemy_bullet_spd;
                    }else
                    if(enemy_bullet_d == 180){
                        enemy_bullet_y = enemy_bullet_y + enemy_bullet_spd;
                    }else
                    if(enemy_bullet_d == 270){
                        enemy_bullet_x = enemy_bullet_x - enemy_bullet_spd;
                    }
                }
            }

            int i = BULL_N;
            while(0 <= --i){
                if (bullets[i].s && bullets[i].d == 0){
                    bullets[i].y = bullets[i].y - bullets[i].s;
                }else
                if (bullets[i].s && bullets[i].d == 90){
                    bullets[i].x = bullets[i].x + bullets[i].s;
                }else
                if (bullets[i].s && bullets[i].d == 180){
                    bullets[i].y = bullets[i].y + bullets[i].s;
                }else
                if (bullets[i].s && bullets[i].d == 270){
                    bullets[i].x = bullets[i].x - bullets[i].s;
                }
            }

            i = BULL_N;
            while(0 <= --i){
                if (bullets[i].x < 0 || bullets[i].x > screenWidth || bullets[i].y < 0 || bullets[i].y > screenHeight){
                    Bullet_init(&bullets[i]);
                }else{
                    // none
                }
            }

            i = BULL_N;
            while(0 <= --i){
                if (bullets[i].s){
                    int x_distance = enemy_x - bullets[i].x;
                    int y_distance = enemy_y - bullets[i].y;
                    if (x_distance > -25 && x_distance < 25 && y_distance > -25 && y_distance < 25 ){
                        enemy_life = enemy_life - 1;
                    }
                }
            }

            {
                if(tank_x < 0){
                    tank_x = 0 + 80;
                }
                if(800 < tank_x){
                    tank_x = 800 - 80;
                }
                if(tank_y < 0){
                    tank_y = 0 + 80;
                }
                if(450 < tank_y){
                    tank_y = 450 - 80;
                }
            }

            {
                int x_distance = tank_x - enemy_bullet_x;
                int y_distance = tank_y - enemy_bullet_y;
                if (x_distance > -25 && x_distance < 25 && y_distance > -25 && y_distance < 25 ){
                    gameover = 1;
                    PlaySound(gameover_sound);
                }
            }

            int player_enemy_x = enemy_x - tank_x;
            int player_enemy_y = enemy_y - tank_y;
            if (enemy_life > 0 && player_enemy_x > -25 && player_enemy_x < 25 && player_enemy_y > -25 && player_enemy_y < 25 ){
                gameover = 1;
                PlaySound(gameover_sound);
            }

            bool got_item = CheckCollisionCircles((Vector2){tank_x, tank_y}, 25, (Vector2){item_x, item_y}, 15);
            if(got_item){
                item_x = -100;
                item_y = -100;
                tank_s = tank_s + 2;
                PlaySound(item_get);
            }

            Landmine_col(&lmine, &enemy_x, &enemy_y, &enemy_life);

            if (enemy_life>0){
                enemy_spd = GetTime() / 15 + 1;
                if (enemy_d == 0){
                    enemy_y = enemy_y - enemy_spd;
                }else
                if (enemy_d == 90){
                    enemy_x = enemy_x + enemy_spd;
                }else
                if (enemy_d == 180){
                    enemy_y = enemy_y + enemy_spd;
                }else
                if (enemy_d == 270){
                    enemy_x = enemy_x - enemy_spd;
                }

                if (enemy_x < 0 || 800 < enemy_x || enemy_y < 0 || 450 < enemy_y){
                    enemy_appear(&enemy_x,&enemy_y,&enemy_d,&enemy_life);
                }
            }else{
                if(rand()%3 == 0){
                    item_x = enemy_x;
                    item_y = enemy_y;
                }

                score += 10;
                if (score >= clearscore){
                    score = clearscore;
                    gameclear = 1;
                    PlaySound(gameclear_sound);
                    clear_time = GetTime();
                }
                else{
                    PlaySound(gekiha);
                    enemy_appear(&enemy_x,&enemy_y,&enemy_d,&enemy_life);
                }
            }
        }else{
            if(IsKeyPressed(KEY_R)){
                ret = 1;
                break;
            }
        }

        int lct = Landmine_col_tank(&lmine, &tank_x, &tank_y);
        if(lct){
            gameover = 1;
            PlaySound(gameover_sound);
        }

        if(0 < score && !gameclear && !gameover){
            if(3 < GetTime() - hera_time){
                score = score - 1;
                hera_time = GetTime();
            }
        }

        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawTexture(background,0,0,WHITE);

            float progress = (float)score / clearscore;
            DrawRectangle(10,10,300,25,GRAY);
            DrawRectangle(10,10,(int)300 * progress,25,GREEN);

            char progressText[20];
            sprintf(progressText,"%d%%",(int)(progress*100));
            DrawText(progressText,350,10,50,BLACK);

            //地雷の表示
            {
                Vector2 origin = {landmine.width / 2.0f,landmine.height / 2.0f};
                Rectangle sourceRect = {0.0f,0.0f,(float)landmine.width,(float)landmine.height};
                Rectangle destRect = {lmine.x,lmine.y,(float)landmine.width,landmine.height};
                DrawTexturePro(landmine, sourceRect, destRect, origin, 0, WHITE);
            }

            //アイテムの表示
            DrawCircle(item_x, item_y, 15, RED);

            //自分の表示
            Vector2 tank_origin = {tank.width / 2.0f,tank.height / 2.0f};
            Rectangle tank_sourceRect = {0.0f,0.0f,(float)tank.width,(float)tank.height};
            Rectangle tank_destRect = {tank_x,tank_y,(float)tank.width,tank.height};
            DrawTexturePro(tank, tank_sourceRect, tank_destRect, tank_origin, tank_d, WHITE);

            //弾の表示
            int i=BULL_N;
            while(0 <= --i){
                Bullet_draw(&bullets[i], &bullet);
            }

            //敵の弾の表示
            {
                Vector2 bullet_origin = {enemy_bullet.width / 2.0f,enemy_bullet.height / 2.0f};
                Rectangle bullet_sourceRect = {0.0f,0.0f,(float)enemy_bullet.width,(float)enemy_bullet.height};
                Rectangle bullet_destRect = {enemy_bullet_x,enemy_bullet_y,(float)enemy_bullet.width,enemy_bullet.height};
                DrawTexturePro(enemy_bullet, bullet_sourceRect, bullet_destRect, bullet_origin, enemy_bullet_d, WHITE);
            }

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
            if (gameclear == 1){
                DrawText("GAMECLEAR",200,150,70,SKYBLUE);
                DrawText(TextFormat("Time:%d Second",clear_time),150,250,70,SKYBLUE);
            }
            if(gameover || gameclear){
                DrawText("Press R Key to replay.", 120, 360, 50, WHITE);
            }
        EndDrawing();
    }
    CloseWindow();
    return ret;
}

int main(void){
    srand(time(NULL));
    InitAudioDevice();

    int r = 1;
    while(r == 1){
        r = main2();
    }
    return r;
}
