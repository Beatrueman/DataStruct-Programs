#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXSIZE 100
#define ERROR -1

typedef struct Node
{
    char data;
    struct Node *leftChild;  // 左子树指针
    struct Node *rightChild; // 右子树指针
} BiTreeNode, *BiTree;

typedef struct
{
    BiTree *base;
    BiTree *top;
    int stacksize;
} SqStack, *Stack;

// 初始化栈
Stack InitStack()
{
    Stack s = (Stack)malloc(sizeof(SqStack));
    if (s == NULL)
    {
        printf("栈初始化失败！\n");
        exit(0);
    }

    s->base = (BiTree *)malloc(MAXSIZE * sizeof(BiTree));
    if (s->base == NULL)
    {
        free(s);
        printf("栈初始化失败！\n");
        exit(0);
    }
    s->top = s->base;
    s->stacksize = MAXSIZE;
    return s;
}

//// 栈操作调试信息
//void PrintStackInfo(Stack s, const char *action)
//{
//    printf("%s 操作: ", action);
//    printf("栈大小: %d, ", s->top - s->base);
//    if (s->top == s->base)
//    {
//        printf("栈为空\n");
//    }
//    else if (s->top[-1] != NULL) // 添加空指针检查
//    {
//        printf("栈顶元素: %c\n", s->top[-1]->data);
//    }
//    else
//    {
//        printf("栈顶元素无效或为空\n");
//    }
//}


// 入栈
void Push(Stack s, BiTree e)
{
    if (s->top - s->base >= s->stacksize)
    {
        printf("栈满!\n");
        return;
    }
    *s->top = e;
    s->top++;
    //PrintStackInfo(s, "Push");
}

// 出栈
void Pop(Stack s, BiTree *e)
{
    if (s->top == s->base)
    {
        printf("栈空！\n");
        return;
    }
    s->top--;
    *e = *s->top;
    //PrintStackInfo(s, "Pop");
}

// 取栈顶元素
BiTree GetTop(Stack s)
{
    if (s->top == s->base)
    {
        printf("栈为空！\n");
        return NULL;
    }
    return *(s->top - 1);
}

// 获取索引
int getIndex(char theta)
{
    switch (theta)
    {
    case '+':
        return 0;
    case '-':
        return 1;
    case '*':
        return 2;
    case '/':
        return 3;
    case '(':
        return 4;
    case ')':
        return 5;
    case '#':
        return 6;
    default:
        return ERROR;
    }
}

// 比较优先级
char getPriority(char theta1, char theta2)
{
    const char priority[][7] =
        {
            {'>', '>', '<', '<', '<', '>', '>'},
            {'>', '>', '<', '<', '<', '>', '>'},
            {'>', '>', '>', '>', '<', '>', '>'},
            {'>', '>', '>', '>', '<', '>', '>'},
            {'<', '<', '<', '<', '<', '=', '0'},
            {'>', '>', '>', '>', '0', '>', '>'},
            {'<', '<', '<', '<', '<', '0', '='},
        };
    int index1 = getIndex(theta1);
    int index2 = getIndex(theta2);
    return priority[index1][index2];
}

// 创建表达式树
BiTree CreateExpTree(BiTree a, BiTree b, char ch)
{
    BiTree T = (BiTree)malloc(sizeof(BiTreeNode));
    if (T == NULL)
    {
        printf("表达式树内存分配失败！\n");
        exit(0);
    }
    T->data = ch;
    T->leftChild = a;
    T->rightChild = b;
    return T;
}


// 创建节点
BiTree CreateNode(char data)
{
    BiTree newNode = (BiTree)malloc(sizeof(BiTreeNode));
    if (newNode == NULL)
    {
        printf("内存分配失败！\n");
        exit(0);
    }
    newNode->data = data;
    newNode->leftChild = NULL;
    newNode->rightChild = NULL;
    return newNode;
}

// 清空栈
void DestroyStack(Stack s)
{
    if(s != NULL) {
        free(s->base);
        s->base = NULL;
        s->top = NULL;
        s->stacksize = 0;
        free(s);
    }
}

// 初始化表达式树
void InitExpTree(char e[], BiTree *root)
{
    char e_copy[MAXSIZE];
    strcpy(e_copy, e);
    strcat(e_copy, "#");

    Stack EXPT = InitStack();
    Stack OPTR = InitStack();

    Push(OPTR, CreateNode('#'));

    char ch;
    BiTree T = NULL, a, b, op_node;
    int i = 0;

    for (i = 0; e_copy[i] != '\0'; i++)
    {
        ch = e_copy[i];
        if (ch >= '0' && ch <= '9')
        {
            T = CreateNode(ch);
            Push(EXPT, T);
        }
        else
        {
            while (getPriority(GetTop(OPTR)->data, ch) == '>')
            {
                Pop(OPTR, &op_node);
                Pop(EXPT, &b);
                Pop(EXPT, &a);
                T = CreateExpTree(a, b, op_node->data);
                Push(EXPT, T);
            }
            if (getPriority(GetTop(OPTR)->data, ch) == '<')
            {
                Push(OPTR, CreateNode(ch));
            }
            else if (getPriority(GetTop(OPTR)->data, ch) == '=')
            {
                Pop(OPTR, &op_node);
            }
        }
    }

    Pop(EXPT, root);
    DestroyStack(EXPT);
    DestroyStack(OPTR);
}

// 表达式计算
int GetValue(int a, int b, char op)
{
    switch (op)
    {
    case '+':
        return a + b;
    case '-':
        return a - b;
    case '*':
        return a * b;
    case '/':
        if (b == 0)
        {
            printf("除数不能为0!\n");
            exit(0);
        }
        return a / b;
    default:
        printf("未知运算符: %c\n", op);
        exit(0);
    }
}

// 后序遍历表达式树求值
int EvaluateExpTree(BiTree T)
{
    if (T == NULL)
    {
        printf("表达式树为空！\n");
        exit(0);
    }

    int lvalue = 0, rvalue = 0;
    if (T->leftChild == NULL && T->rightChild == NULL)
    {
        return T->data - '0'; // 如果结点为操作数，则返回该结点的数值
    }
    else
    {
        lvalue = EvaluateExpTree(T->leftChild);
        rvalue = EvaluateExpTree(T->rightChild);

        int result = GetValue(lvalue, rvalue, T->data);
        return result;
    }
}

// 释放root
void FreeTree(BiTree root)
{
    if(root != NULL) {
        FreeTree(root->leftChild);
        FreeTree(root->rightChild);
        free(root);
    }
}


int main()
{
    printf("请输入表达式，最后以 = 结束所有表达式输入:\n");
    int results[MAXSIZE];
    int count = 0;
    while(1)
    {
        char e[MAXSIZE];
        gets(e);
        int len = strlen(e);
        if (len > 0 && e[len - 1] == '=') {
            e[len - 1] = '\0';  // 将 '=' 替换为字符串结束符
        }

        if(len == 1) {
            break;
        }

        BiTree root = NULL;
        InitExpTree(e, &root);
        int result = EvaluateExpTree(root);
        results[count++] = result;
    }
    printf("表达式的值为\n");
    int i = 0;
    for(i; i < count; i++) {
      printf("%d\n", results[i]);
    }
    return 0;
}

