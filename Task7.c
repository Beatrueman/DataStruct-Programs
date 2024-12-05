#include <stdio.h>
#include <stdlib.h>
#define MVNum 100
#define MaxInt 32767
#define ERROR -1

typedef int VertexType;

typedef struct
{
    int *base; // 队列存储空间
    int front;
    int rear;
} SqQueue, *Queue;

typedef struct
{
    VertexType vex[MVNum];     // 顶点表
    int arcs[MVNum][MVNum];    // 邻接矩阵，无向图
    int Vexnum;                // 顶点数
    int arcnum;                // 边数
} AMGraph;

// 初始化队列
Queue InitQueue()
{
    Queue q = (Queue)malloc(sizeof(SqQueue));
    q->base = (int *)malloc(MVNum * sizeof(int));
    if (!q->base)
    {
        printf("队列初始化失败!\n");
        exit(0);
    }
    q->rear = 0;
    q->front = q->rear;
    return q;
}

// 入队
int Push(Queue q, int e)
{
    if ((q->rear + 1) % MVNum == q->front)
        return ERROR;
    q->base[q->rear] = e;          // 新元素插入队尾
    q->rear = (q->rear + 1) % MVNum; // 队尾指针加1
    return 0;
}

// 出队
int Pop(Queue q, int *e)
{
    if (q->rear == q->front)
        return ERROR;
    *e = q->base[q->front];         // 保存队头元素
    q->front = (q->front + 1) % MVNum; // 队头指针加1
    return 0;
}

// 创建无向图
int CreateAMGraph(AMGraph *G)
{
    int i, j;
    scanf("%d %d", &G->Vexnum, &G->arcnum); // 输入顶点数和边数
    if (G->Vexnum == 0 && G->arcnum == 0) // 输入结束标志
        return 0;

    for (i = 0; i < G->Vexnum; ++i)
        G->vex[i] = i + 1; // 顶点编号从1开始

    // 初始化邻接矩阵
    for (i = 0; i < G->Vexnum; i++)
        for (j = 0; j < G->Vexnum; j++)
            G->arcs[i][j] = 0;

    // 构建邻接矩阵
    for (int k = 0; k < G->arcnum; ++k)
    {
        int v1, v2;
        scanf("%d %d", &v1, &v2);
        --v1;
        --v2; // 调整索引，因为C数组是从0开始的
        G->arcs[v1][v2] = 1;
        G->arcs[v2][v1] = 1; // 无向图
    }
    return 1;
}

// BFS遍历图，计算到目标顶点的距离
float BFS(AMGraph *G, int s)
{
    int *len = (int *)malloc(sizeof(int) * G->Vexnum); // 记录到s的距离
    int state[G->Vexnum];                             // 记录顶点是否被访问
    for (int i = 0; i < G->Vexnum; i++)
    {
        state[i] = 0;     // 未访问
        len[i] = MaxInt; // 初始距离设为无穷大
    }

    Queue Q = InitQueue();
    Push(Q, s - 1);    // s从1开始
    state[s - 1] = 1;  // 标记已访问
    len[s - 1] = 0;    // 起始顶点距离为0

    while (Q->front != Q->rear)
    {
        int u;
        Pop(Q, &u);
        for (int j = 0; j < G->Vexnum; j++)
        {
            if (G->arcs[u][j] && !state[j])
            {
                state[j] = 1;       // 标记已访问
                len[j] = len[u] + 1; // 更新距离
                Push(Q, j);
            }
        }
    }

    int rs = 0;
    for (int i = 0; i < G->Vexnum; i++)
    {
        if (len[i] <= 6) // 只统计到6步的顶点
            rs++;
    }

    free(len);
    free(Q->base);
    free(Q);
    return (float)rs / G->Vexnum * 100;
}

int main()
{
    AMGraph G;
    int count = 0; // 记录输入的图的个数
    AMGraph graphs[10]; // 假设最多有 10 个图

    // 读取输入数据，直到 0 0
    while (1)
    {
        if (!CreateAMGraph(&G))
            break;
        graphs[count++] = G; // 将当前图保存到数组中
    }

    // 统一输出所有图的结果
    for (int g = 0; g < count; g++)
    {
        for (int i = 0; i < graphs[g].Vexnum; i++)
        {
            float p = BFS(&graphs[g], i + 1);
            printf("%d: %.2f%%\n", i + 1, p);
        }
        if (g != count - 1) // 每个图之间输出一个空行
            printf("\n");
    }

    return 0;
}
