#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 输入DNA序列，因为病毒为环状，所以病毒DNA需要拼接
void PreProcess(char h[], char v[], int *len_h, int *len_v, int *num, char patterns[][200])
{
    //printf("输入病毒DNA序列:");

    *len_v = strlen(v);

    // 对病毒环状DNA拼接
    char *v_double = (char *)malloc(strlen(v) * 2 + 1);
    if (v_double == NULL) {
        printf("内存分配失败！\n");
        exit(1);
    }
    strcpy(v_double, v); // 病毒DNA复制到新数组
    strcat(v_double, v); // 病毒DNA追加到末尾

    *len_h = strlen(h);

    // 生成所有可能的旋转形式
    *num = *len_v;
    int i = 0;
    for(i; i < *num; i++) {
        strncpy(patterns[i], v_double + i, *len_v);
        patterns[i][*len_v]= '\0';
    }

    free(v_double);
}

// 获取next数组
void GetNext(int next[], char v[], int len_v)
{
    int i = 0, j = -1;
    next[0] = -1; // next初始化为-1

    while (i < len_v - 1) {
        if ((j == -1) || (v[i] == v[j])) {
            i++;
            j++;
            next[i] = j;
        } else {
            j = next[j];
        }
    }
}

// KMP算法
int kmp(char h[], char v[], int len_v)
{
    int *next = (int *)malloc(len_v * sizeof(int));
    if (next == NULL) {
        printf("next数组内存分配失败！");
        exit(0);
    }
    GetNext(next, v, len_v);

    int i = 0, j = 0;
    int len_h = strlen(h);
    while (i < len_h && j < len_v ) {
        if (j == -1 || h[i] == v[j]) {
            i++;
            j++;
        } else {
            j = next[j]; // 根据next数组回退
        }
    }

    free(next);

    return j == len_v;
}

int main()
{
    char h[100], v[200];
    char patterns[100][200];
    int len_h, len_v, num;
    int count = 0;
    char result[100][10];

    while(1) {
        scanf("%s %s", v, h);

        if(strcmp(v, "0") == 0 &&strcmp(h, "0") == 0 ) {
            printf("\n");
            break;
        }

        PreProcess(h, v, &len_h, &len_v, &num, patterns);

        int i = 0, found = 0;
        for (i; i < num; i++) {
            if (kmp(h, patterns[i], len_v)) {
                found = 1;
                break;
            }
        }

        if (found) {
            strcpy(result[count], "YES");
        } else {
            strcpy(result[count], "NO");
        }
        count++;
    }

    int k = 0;
    for(k; k < count; k++)
        puts(result[k]);
    return 0;
}
