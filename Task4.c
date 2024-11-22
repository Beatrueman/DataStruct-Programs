#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ERROR -1
#define MAX_SIZE 100

// 哈夫曼树
typedef struct
{
    int weight; // 结点权重
    int parent, lchild, rchild; // 结点的双亲、左孩子、右孩子
}HTNode, *HFTree;

// 哈夫曼编码表
typedef char **HuffmanCode;

// 统计字符出现的次数
void Weight(char *str, int count[])
{
    // 初始化count数组为0
    for (int i = 0; i < 26; i++) {
        count[i] = 0;
    }
    int i;
    for(i = 0; i < strlen(str); i++) {
        count[str[i] - 'a']++;
    }
}

// 选择函数，选择权值小的两个结点构造哈夫曼树
void Select(HTNode HT[], int m, int *s1, int *s2)
{
    int min1 = 10000, min2 = 10000;
    *s1 = 1;
    *s2 = 1;
    int i;
    for(i = 1; i <= m; i++) {
        if(HT[i].parent == 0) {
            if(HT[i].weight < min1) {
                min2 = min1;
                *s2 = *s1;
                min1 = HT[i].weight;
                *s1 = i;
            }
            else if(HT[i].weight < min2) {
                min2 = HT[i].weight;
                *s2 = i;
            }
        }
    }
}
// 构建哈夫曼树
void CreateHuffmanTree(HFTree *HT, int count[], int n)
{
    if(n <= 1)
        return;

    int m = 2 * n - 1;
    int i;
    *HT = (HFTree)malloc((m + 1) * sizeof(HTNode));
    if(*HT == NULL) {
        printf("内存分配失败！");
        exit(ERROR);
    }

    // 将1到m单元中的双亲、左右孩子的下标都初始化为0
    for(i = 1; i <=m; i++) {
        (*HT)[i].parent = 0;
        (*HT)[i].lchild = 0;
        (*HT)[i].rchild = 0;
    }

    // 初始化叶子的权值为count
    int index = 1;
    for(i = 0; i < 26; i++) {
        if(count[i] > 0)
            (*HT)[index++].weight = count[i];
    }

    // 构造哈夫曼树
    for(i = n + 1; i <= m; i++)
    {
        int s1, s2;
        Select(*HT, i - 1, &s1, &s2);

        (*HT)[s1].parent = i;
        (*HT)[s2].parent = i;
        
        (*HT)[i].lchild = s1;
        (*HT)[i].rchild = s2;
        (*HT)[i].weight = (*HT)[s1].weight +(*HT)[s2].weight;
    }
}

// 哈夫曼编码
void CreateHuffmanCode(HFTree HT, HuffmanCode *HC, int n)
{
    *HC = (HuffmanCode)malloc((n + 1) * sizeof(char*));
    char *temp = (char *)malloc(n * sizeof(char));
    temp[n - 1] = '\0';

    int i;
    for(i = 1; i <= n; ++i)
    {
        int start = n - 1, c = i, p = HT[i].parent;
        while(p != 0) {
            if(HT[p].lchild == c)
                temp[--start] = '0';
            else
                temp[--start] = '1';
            c = p;
            p = HT[p].parent;
        }
        (*HC)[i] = (char *)malloc((n - start) * sizeof(char));
        strcpy((*HC)[i], &temp[start]);
    }
    free(temp);
}

// 编码字符串
void Encode(HuffmanCode HC, int count[], char *str, char *encoded)
{
    encoded[0] = '\0'; // 初始化编码字符串为空
    for(int i = 0; str[i] != '\0'; i++) {
        if(str[i] >= 'a' && str[i] <= 'z') {
            strcat(encoded, HC[str[i] - 'a' + 1]);
        }
    }
}

// 译码字符串
void Decode(HFTree HT, char s[], char a[], int n)
{
    int i = 0;
    int f = 2 * n - 1;
    while(s[i] != '\0') {
        if(s[i] == '0')
            f = HT[f].lchild;
        else
            f = HT[f].rchild;
        if(f <= n) {
            printf("%c", a[f]);
            f = 2 * n - 1;
        }
        i++;
    }
}

int main()
{
    HFTree HT = NULL;
    HuffmanCode HC;
    char str[MAX_SIZE][MAX_SIZE];
    int count[26];

    // 输入
    int m;
    for(m = 0; m < MAX_SIZE; m++) {
        gets(str[m]);
        if(strcmp(str[m], "0") == 0)
            break;
    }

    // 输出
    // 二维字符数组只有前i行有内容
    for(int j = 0; j < m; j++) {

        // 统计次数
        Weight(str[j], count);

        int n = 0;
        for(int i = 0; i < 26; i++) {
            if(count[i] > 0)
                n++;
        }
        // 创建哈夫曼树
        CreateHuffmanTree(&HT, count, n);

        // 生成哈夫曼编码
        CreateHuffmanCode(HT, &HC, n);

        printf("\n");
        // 输出字符频率
        for(int k = 0; k < 26; k++) {
            if(count[k] != 0) {
                printf("%c:%d ", k + 'a', count[k]);
            }
        }
        printf("\n");

        // 哈夫曼树的终态
        for(int i = 1; i <= 2 * n - 1; i++) {
            printf("%d %d %d %d %d\n", i, HT[i].weight, HT[i].parent, HT[i].lchild, HT[i].rchild);
        }

        // 输出单个字符的编码
        for(int k = 0; k < 26; k++) {
            if(count[k] != 0) {
                printf("%c:%s ", k + 'a', HC[k + 1]);
            }
        }
        printf("\n");

        // 编码字符串
        char encoded[100];
        Encode(HC, count, str[j], encoded);
        printf("%s\n", encoded);

        // 初始化字符数组 a
        char a[100];
        for(int i = 1, k = 0; i <= n; i++, k++) {
            if(count[k] > 0) {
                a[i] = k + 'a';
            }
        }
        // 译码字符串
        Decode(HT, encoded, a, n);

        free(HT);
        for(int i = 1; i <= n; i++)
            free(HC[i]);
        free(HC);
}

    return 0;
}
