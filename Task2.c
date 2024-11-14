#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXSIZE 100
#define ERROR -1

// 运算符栈
typedef struct
{
    char *base;
    char *top;
    int stacksize;
}OpStack, *opStack;

// 操作数栈
typedef struct {
    int *base;
    int *top;
    int stacksize;
}NumStack, *numStack;

// 初始化操作数栈为空栈
numStack InitNumStack()
{
    numStack s = (numStack)malloc(sizeof(NumStack));
    if(s == NULL) {
        printf("操作数栈初始化失败！\n");
        exit(0);
    }

    // 为栈底分配内存
    s->base = (int *)malloc(MAXSIZE * sizeof(int));
    if(s->base == NULL) {
        printf("操作数栈内存初始化失败！\n");
        free(s);
        exit(0);
    }
    s->top = s->base;
    s->stacksize = MAXSIZE;
    return s;
}

// 初始化运算符栈为空栈
opStack InitOpStack()
{
    opStack s = (opStack)malloc(sizeof(OpStack));
    if(s == NULL) {
        printf("运算符栈初始化失败！\n");
        exit(0);
    }

    // 为栈底分配内存
    s->base = (char *)malloc(MAXSIZE * sizeof(char));
    if(s->base == NULL) {
        printf("运算符栈内存初始化失败！\n");
        free(s);
        exit(0);
    }
    s->top = s->base;
    s->stacksize = MAXSIZE;
    return s;
}

// 入栈
int PushNum(numStack s, int e)
{
    if(s->top - s->base == s->stacksize) {
        printf("操作数栈满！\n");
        return ERROR;
    }
    *s->top = e;
    s->top++;
    return 0;
}

// 运算符入栈
int PushOp(opStack s, char e) {
    if (s->top - s->base == s->stacksize) {
        printf("运算符栈满！\n");
        return ERROR;
    }
    *s->top = e;
    s->top++;
    return 0;
}

// 出栈
int PopOp(opStack s, char *e)
{
    if(s->top == s->base) {
        printf("运算符栈空！\n");
        return ERROR;
    }
    s->top--;
    *e = *(s->top);
    return 0;
}

// 操作数出栈
int PopNum(numStack s, int *e)
{
    if(s->top == s->base) {
        printf("操作数栈空！\n");
        return ERROR;
    }
    s->top--;
    *e = *(s->top);
    return 0;
}

// 取栈顶元素
char GetOpTop(opStack s)
{
    if (s->top == s->base) {
        printf("运算符栈空！\n");
        exit(0);
    }
    return *(s->top - 1);
}

// 获取索引
int getIndex(char theta)
{
    switch(theta)
    {
        case '+': return 0;
        case '-': return 1;
        case '*': return 2;
        case '/': return 3;
        case '(': return 4;
        case ')': return 5;
        case '#': return 6;
        default:  return -1;
    }
}

// 获取优先级
char getPriority(char theta1, char theta2)
{
    const char priority[][7] =
    {
            { '>','>','<','<','<','>','>' },
            { '>','>','<','<','<','>','>' },
            { '>','>','>','>','<','>','>' },
            { '>','>','>','>','<','>','>' },
            { '<','<','<','<','<','=','0' },
            { '>','>','>','>','0','>','>' },
            { '<','<','<','<','<','0','=' },
    };
    int index1 = getIndex(theta1);
    int index2 = getIndex(theta2);
    if (index1 == -1 || index2 == -1)
        return '0';
    return priority[index1][index2];
}

// 计算函数
int Operate(int a, int b, char op)
{
    switch(op)
    {
        case '+':
            return a + b;
        case '-':
            return a - b;
        case '*':
            return a * b;
        case '/':
            if(b == 0){
                printf("分母不能为0！");
                exit(0);
            }
            return a / b;
        default:
            printf("操作符有误！\n");
            return 0;
    }
}

// 主计算函数
int Calculate(char e[])
{
    numStack OPND = InitNumStack();
    opStack OPTR = InitOpStack();
    PushOp(OPTR, '#');  // 初始化运算符栈

    char ch, op;
    int a, b, result = 0;
    int num = 0;
    int flag = 0; // 检测是否为多位数

    strcat(e, "#");

    for(int i = 0; e[i] != '\0'; i++)
    {
        ch = e[i];
        if (ch >= '0' && ch <= '9') {
            num = num * 10 + (ch - '0'); // 累加组合成多位数
            flag = 1;
        }
        else {
            if(flag) {
                PushNum(OPND, num);  // 压入操作数
                num = 0;
                flag = 0;
            }

            while(getPriority(GetOpTop(OPTR), ch) == '>') {
                PopOp(OPTR, &op);
                PopNum(OPND, &b);
                PopNum(OPND, &a);
                result = Operate(a, b, op);
                PushNum(OPND, result);
            }
            if (getPriority(GetOpTop(OPTR), ch) == '<') {
                PushOp(OPTR, ch);  // 压入当前运算符
            } else if (getPriority(GetOpTop(OPTR), ch) == '=') {
                PopOp(OPTR, &op); // 出栈匹配
            }
        }
    }
    PopNum(OPND, &result);
    return result;
}

int main()
{
    char e[MAXSIZE]; // 一条表达式
    char p[MAXSIZE][MAXSIZE]; // 所有表达式
    int count = 0;

    printf("输入表达式，每行一个，以\"=\"结束：\n");
    while(1) {
        gets(e); // 读取一行表达式
        // 去除换行符
        e[strcspn(e,"\n")] = 0;

        if(strcmp(e, "=") == 0) {
            break;
        }

        strcpy(p[count++], e);
    }
    printf("计算结果为：\n");
    for(int i = 0; i <count; i++) {
        int result = Calculate(p[i]);
        printf("%d\n", result);
    }
    return 0;
}
