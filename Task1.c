#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXSIZE 10000
#define ERROR -1

// 图书信息定义
typedef struct
{
    char no[20]; // 图书 ISBN
    char name[50]; // 图书名字
    float price; // 图书价格
}Book;

// 图书信息表定义
typedef struct LNode
{
    Book data; // 结点的数据域
    struct LNode *next; // 结点指针域
}LNode, *LinkList;

// 初始化链表
LinkList InitList()
{
    LinkList L = (LinkList)malloc(sizeof(LNode));
    if(L == NULL) {
        printf("链表初始化失败！\n");
        exit(0);
    }
    L->next = NULL; // 头指针设置为空
    printf("链表初始化成功！\n");
    return L;
}

// 后插法创建链表
int CreateList_R(LinkList *L)
{
    *L = InitList();
    LinkList r = *L; // 尾指针r指向头结点

    Book book;
    while(1)
    {
        LinkList p = (LinkList)malloc(sizeof(LNode));
        if(p == NULL)
        {
            printf("创建链表时分配内存失败！\n");
            return ERROR;
        }
        scanf("%s %s %f", &book.no, &book.name, &book.price);
        // 吸收回车
        getchar();
        // 检查输入结束标志
        if(strcmp(book.no, "0") == 0 &&strcmp(book.name, "0") == 0 &&book.price == 0) {
            printf("\n");
            break;
        }
        p->data = book;
        p->next = NULL; // 新结点设置为尾结点，也就是新结点的指向NULL
        r->next = p; // 原来的尾结点指向新结点
        r = p; // 尾指针指向新结点
    }
    return 0;
}

// 获取链表长度
int GetLength(LinkList L)
{
    LinkList p = L->next;
    int length = 0;
    while(p != NULL) {
        ++length;
        p = p->next;
    }
    return length;
}

// 修改
void ChangePrice(LinkList L)
{
    float sum = 0, average = 0;
    int length = GetLength(L);
    LinkList p = L->next; // p指向首元节点
    while(p != NULL)
    {
        sum += p->data.price;
        p = p->next;
    }
    average = sum / length;
    printf("平均价格: %.2f\n", average);
    //printf("链表长度: %d", length);

    // 重新将p指向头结点
    p = L->next;
    while(p != NULL)
    {
        if( p->data.price < average )
            p->data.price *= 1.2;
        else if( p->data.price > average )
            p->data.price *= 1.1;
        p = p->next;
    }
    PrintList(L);
}

// 查找最贵图书
void FindHighPrice(LinkList L)
{
    float max = 0;
    int count = 0;
    LinkList p = L->next;
    // 找最大价格
    while(p != NULL)
    {
        if(p->data.price > max) {
            max = p->data.price;
            count = 1;
        }
        else if(p->data.price == max)
        {
            ++count;
        }
        p = p->next;
    }

    printf("%d\n", count);

    // 找最贵价格图书信息
    p = L->next; // 重置p到首元节点
    while(p != NULL)
    {
        if(p->data.price == max)
        {
            printf("%s %s %.2f\n", p->data.no, p->data.name, p->data.price);
        }
        p = p->next;
    }
}

// 新图书入库(插入)
int BookInsert(LinkList *L, int i, Book e)
{
    // 头指针
    LinkList p = *L;
    int j = 0;
    // 找第i-1个位置
    while(p && (j < i - 1))
    {
        p = p->next;
        ++j;
    }
    if(!p || j > i - 1)
        return ERROR;
    LinkList s = (LinkList)malloc(sizeof(LNode)); // 创建新结点
    if(s == NULL)
        return ERROR;
    s->data = e;
    s->next = p->next;
    p->next = s;
    return 0;
}

// 新图书的出库(删除)
int BookDelete(LinkList *L, int i)
{
    // 头指针
    LinkList p = *L;
    int j = 0;
    // 找第i-1位置
    while( (p->next) && (j < i - 1) )
    {
        p = p->next;
        ++j;
    }

    // 当i大于链表长度或者i<1时，位置不合理
    if( !(p->next) || (j > i - 1) )
        return ERROR;
    LNode *q = p->next; // 临时保存被删除结点的地址;
    p->next = q->next;
    free(q);
    return 0;

}

// 根据ISBN对图书去重
void BookDuplicateDeleteByISBN(LinkList *L)
{
    char ISBNList[MAXSIZE][20]; // 存储ISBN
    int count = 0;
    // p指向首元节点
    LinkList p = (*L)->next;
    LinkList pre = *L; // 用来保存前驱结点

    while( p != NULL )
    {
        int flag = 0;
        for(int i = 0; i < count; i++)
        {
            if(strcmp(p->data.no, ISBNList[i]) == 0) {
                flag = 1; // 找到，标志设置为1
                break;
            }
        }

        if(flag) {
            LinkList temp = p;
            pre->next = p->next; // 前驱指针跳过当前结点
            p = p->next;
            free(temp);
        }
        else {
            // 如果当前书号没出现过，加入ISBN字符数组里
            strcpy(ISBNList[count], p->data.no);
            ++count; // 已访问书号数量增加
            pre = p; // 更新前驱结点为当前结点
            p = p->next; // 移动到下一个结点
        }
    }
}

// 输出
void PrintList(LinkList L)
{
    int length = GetLength(L);
    printf("\n%d\n", length);
    // p重新指向头结点
    LinkList p = L->next;
    while(p != NULL)
    {
        printf("%s %s %.2f\n", p->data.no, p->data.name, p->data.price);
        p = p->next;
    }
}

int main()
{
    LinkList L;

    printf("======欢迎使用图书信息管理系统======\n");
    printf("1.图书信息表的创建与输出\n");
    printf("2.图书信息表的修改\n");
    printf("3.查找最贵图书\n");
    printf("4.图书入库\n");
    printf("5.图书出库\n");
    printf("6.图书去重\n");
    printf("7.退出\n");
    printf("========================\n");
    printf("请选择序号：");
    int n;
    int result;
    scanf("%d", &n);

    switch(n) {
        case 1:
            result = CreateList_R(&L);
            if(result == 0) {
                PrintList(L);
                printf("\n");
                printf("创建链表成功！\n");
            }
            else
            {
                printf("创建链表失败！\n");
            }
            break;
        case 2:
            result = CreateList_R(&L);
            if(result == 0) {
                ChangePrice(L);
                printf("\n");
                printf("修改链表成功！\n");
            }
            else
            {
                printf("创建链表失败！\n");
            }
            break;
        case 3:
            result = CreateList_R(&L);
            if(result == 0) {
                FindHighPrice(L);
                printf("\n");
            }
            else
            {
                printf("创建链表失败！\n");
            }
            break;
        case 4:
            result = CreateList_R(&L);
            if(result == 0) {
                int pos;
                printf("请输入入库图书的位置：");
                scanf("%d", &pos);
                Book newBook;
                printf("请输入入库图书的信息：");
                scanf("%s %s %f", &newBook.no, &newBook.name, &newBook.price);
                getchar();
                int r = BookInsert(&L, pos, newBook);
                if( r == 0 )
                    PrintList(L);
                else
                    printf("抱歉，入库位置非法!");
                printf("\n");
            }
            else
                printf("创建链表失败！\n");
            break;
        case 5:
            result = CreateList_R(&L);
            if(result == 0) {
                int pos;
                printf("请输入出库图书的位置：");
                scanf("%d", &pos);
                int r = BookDelete(&L, pos);
                if( r == 0 )
                    PrintList(L);
                else
                    printf("抱歉，出库位置非法!");
                printf("\n");
            }
            else
                printf("创建链表失败！\n");
            break;
        case 6:
            result = CreateList_R(&L);
            if(result == 0) {
                BookDuplicateDeleteByISBN(&L);
                PrintList(L);
            }
            else
                printf("创建链表失败！\n");
            break;
        case 7:
            printf("感谢使用，再见！");
            exit(0);
    }
    return 0;
}
