#include<stdio.h>
#include<string.h>

typedef struct//構造体テンプレート(設計図)
{
    int id;//出席番号
    char name[256];//名前
    int score;//点数
} student;

int main(void)
{
    student a;//構造体変数
    a.id = 1;
    strcpy(a.name,"A");
    a.score = 400;

    printf("id: %d\n",a.id);
    printf("name: %s\n",a.name);
    printf("score: %d\n",a.score);
}
