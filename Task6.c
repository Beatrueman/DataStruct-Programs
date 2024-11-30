#include <stdio.h>
#include <stdlib.h>
#define ERROR -1
#define MVNum 100
#define MaxInt 32767

// https://blog.csdn.net/Attract1206/article/details/106841002

typedef char VertexType;

typedef struct
{
    VertexType vex[MVNum]; // 顶点表
    int arcs[MVNum][MVNum]; // 邻接矩阵，权重为整数
    int Vexnum; //顶点数
    int arcnum; // 边数
}MGraph;


int LocateVex(MGraph *G, VertexType v)
{
    for(int i = 0; i < G->Vexnum; i++) {
        if(v == G->vex[i]) {
            return i;
        }
    }

    printf("No such Vertex!");
    return -1;
}

void CreateGraph(MGraph *G)
{
    int i, j;

    // 输入顶点数和边数
    printf("输入顶点个数和边数：\n");
    printf("顶点数：");
    scanf("%d", &G->Vexnum);
    printf("边数：");
    scanf("%d", &G->arcnum);

    // 输入顶点元素
    printf("输入顶点元素:\n");
    scanf("%s", G->vex);
    printf("\n");

    // 邻接矩阵初始化
    for(i = 0; i < G->Vexnum; i++) {
        for(j = 0; j < G->Vexnum; j++) {
            G->arcs[i][j] = MaxInt;
        }
    }

    // 构建邻接矩阵
    int n, m;
    VertexType v1, v2;
    int w; // v1到v2的权值

    printf("输入路径及路径长度：\n");
    for(int i = 0; i < G->arcnum; i++) {
        printf("输入第 %d 条路径信息：", i + 1);
        scanf(" %c%c, %d", &v1, &v2, &w);
        n = LocateVex(G, v1);
        m = LocateVex(G, v2);

        if(n == -1 || m == -1) {
            printf("No this Vertex!\n");
            return;
        }

        // 赋权值
        G->arcs[n][m] = w;
    }
}

// 打印图
void Print(MGraph G)
{
    int i, j;
    printf("\n 邻接矩阵 \n");
    printf("\t");
    for(i = 0; i < G.Vexnum; i++)
        printf("\t%c", G.vex[i]);
    printf("\n");

    for(i = 0; i < G.Vexnum; i++) {
        printf("\t%c", G.vex[i]);

        for(j = 0; j < G.Vexnum; j++) {
            if(G.arcs[i][j] == MaxInt) {
                printf("\t∞");
            }
            else {
                printf("\t%d", G.arcs[i][j]);
            }
        }
        printf("\n");
    }
}

void displayPath(int dist[], int path[], MGraph *G, VertexType start)
{
    int i, j, k = 0;
    int temp[MVNum]; // 临时数组
    //VertexType target; // 目标地点
    int loc = 0;

    for(i = 0; i < MVNum; i++)
        temp[i] = -1;
    printf("\n......\n");
    printf("结果展示:\n");
    printf("\n\n");
    // 打印dist[]
    printf("\tdist[i]:\n\t");
    for(i = 0; i < G->Vexnum; i++)
        printf("\t%d", i);
    printf("\n\t");
    for(i = 0; i < G->Vexnum; i++)
        printf("\t%d", dist[i]);
    printf("\n");

    printf("\n\n");
    // 打印path[]
    printf("\n\tpath[i]:\n\t");
    for(i = 0; i < G->Vexnum; i++)
        printf("\t%d", i);
    printf("\n\t");
    for(i = 0; i < G->Vexnum; i++)
        printf("\t%d", path[i]);
    printf("\n");

    printf("\n\n");
    // 最短路径
    printf("最短路径:\n\n");
    for(i = 0; i < G->Vexnum; i++) {
        loc = i;
        j = 0;
        while(loc != -1)
        {
            temp[j] = loc;
            loc = path[loc];
            j++;
        }

        if(j - 1 == 0 && G->vex[temp[j-1]] == start)
        {
            printf("\t");
            printf("%c->%c：%c为起始点！", start, G->vex[i], G->vex[temp[j-1]]);
        }
        else if(j - 1 == 0 && G->vex[temp[j-1]] != start)
        {
            printf("\t");
            printf("%c->%c：%c不可达！", start, G->vex[i], G->vex[temp[j-1]]);
        }
        else {
            printf("\t");
            printf("%c->%c：", start, G->vex[i]);
            for(j = j - 1; j >= 0; j--)
                printf("%c ", G->vex[temp[j]]);
            printf("(总路径长度:%d)", dist[i]);
        }
        for(k = 0; k <20; k++)
            temp[k] = -1;
        printf("\n\n");
    }
}

int FindMinDist(int dist[], int s[], int vexnum)
{
    int i;
    int loc;
    int min = MaxInt + 1;
    for(i = 0; i < vexnum; i++)
    {
        if(s[i] == 0)
        {
            if(dist[i] < min)
            {
                min = dist[i];
                loc = i;
            }
        }
    }
    return loc;
}

void Dijkstra(MGraph *G, VertexType start)
{
    int i, num;
    int loc;
    int min;
    int dist[MVNum]; // 最短路径长度数组
    int path[MVNum]; // 最短路径数组
    int s[MVNum]; // 集合S（1表示该顶点已处理，属于集合S； 0代表未处理，不属于集合S， 属于集合V-S）

    // 初始化dist和path
    loc = LocateVex(G, start);
    for(i = 0; i < G->Vexnum; i++)
    {
        dist[i] = G->arcs[loc][i];
        if(dist[i] != MaxInt)
            path[i] = loc;
        else
            path[i] = -1;

    }

    // 初始化S数组
    for(i = 0; i < G->Vexnum; i++)
        s[i] = 0;
    s[loc] = 1;
    num = 1;

    // 第3步
    while(num < G->Vexnum)
    {
        // 在dist数组中查找其对应的s[i]=0,即未处理的最小值元素
        min = FindMinDist(dist, s, G->Vexnum);
        // 将找到的最短边所对应的顶点加入集合S
        s[min] = 1;

        // 加入新的顶点后，更新dist和path
        for(i = 0; i < G->Vexnum; i++)
        {
            if((s[i] == 0) && (dist[i] > dist[min] + G->arcs[min][i]))
            {
                dist[i] = dist[min] + G->arcs[min][i];
                path[i] = min;
            }
        }
        num++;
    }
    displayPath(dist, path, G, start);
}

int main()
{
    MGraph G;
    VertexType start;

    CreateGraph(&G);
    Print(G);

    printf("输入起始点：");
    scanf(" %c", &start);
    printf("\n");
    Dijkstra(&G, start);

    return 0;
}
