#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXSIZE 100

typedef struct
{
    char name[8]; // 姓名
    float score; // 分数
}student;

// 冒泡排序
void BubbleSort(student *s, int n)
{
    for(int i = 0; i < n - 1; i++) {
        for(int j = 0; j < n - 1 - i; j++) {
            if(s[j].score < s[j+1].score) {
                student temp = s[j];
                s[j] = s[j+1];
                s[j+1] = temp;
            }
        }
    }
}

// 打印学生信息
void Print(student *s, int n)
{
    for(int i = 0; i < n; i++)
        printf("%d %s %.2f\n", i + 1, s[i].name, s[i].score);
}

// 输入学生信息
void Input(student *s, int *n)
{
    for(int i = 0; i < MAXSIZE; i++) {
        scanf("%s %f", s[i].name, &s[i].score);
        (*n)++; // 学生数加1
        if(strcmp(s[i].name, "0") == 0 && s[i].score == 0.0f) {
            (*n)--; // 因为 "0 0" 不算作一个有效学生记录，所以减去这次增量
            break;
        }
    }
}

int main()
{
    student s[MAXSIZE];
    int num = 0;

    Input(s, &num);
    BubbleSort(s, num);
    Print(s, num);

    return 0;
}

