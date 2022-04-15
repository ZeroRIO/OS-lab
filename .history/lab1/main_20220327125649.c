#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct map
{
    unsigned size; // 空闲区大小
    char *addr; // 空闲区地址
    struct map *p, *n; // 前一个空闲区和后一个空闲区
};

struct map *lmalloc(struct map *current, unsigned size)  //分配函数
{
    char *a = current->addr;
    struct map *pre, *next;
    struct map *tmp = current;
    if ((size <= 0) || (size > 1000))
    {
        printf("只能分配大小不超过1000的空间。\n");
        return current;
    }
    if (current->size >= size)  //当前区空间够大
    {
        current->addr += size;
        current->size -= size;
        if (current->size == 0)
        {
            pre = current->p;
            next = current->n;
            next->p = pre;
            pre->n = next;
            current = next;
            return current->n;
        }   
    }
    else   //当前区的空间不够
    {
        tmp = tmp->n;
        while (tmp != current)
        {
            if (tmp->size >= size)
            {
                current = tmp;
                current->addr += size;
                current->size -= size;
                if (current->size == 0)
                {
                    pre = current->p;
                    next = current->n;
                    next->p = pre;
                    pre->n = next;
                    current = next;
                }
                return current->n;
            }
            else tmp = tmp->n;
        }
        printf("空间不够，无法分配！\n");
    }
    return current->n;
}

char *lfree(struct map *current, unsigned size, char *addr)
{
    struct map *tmp = current;
    struct map *next, *pre;
    char *a = addr;
    if ((size <= 0) || (size > 1000))
    {
        printf("只能释放不超过1000的空间。\n");
        return a;
    }
    do
    {
        tmp = tmp->n;
        if (((tmp->p->addr <= a) && (tmp->addr >= a)) || (tmp->p == tmp)
            || ((tmp->p->addr > a) && (tmp->p->p->addr >= tmp->p->addr)))
            break;
    } while (tmp != current);

    if (tmp->p == tmp)
    {
        if ((tmp->addr <= a) && (tmp->addr + tmp->size >= a))
        {
            printf("该区域不需要释放。\n");
            return a;
        }
    }
    else if (((tmp->p->addr + tmp->p->size) > a) || (a + size > tmp->addr))
    {
        printf("该区域不需要释放\n");
        return a;
    }

    if ((tmp->p->addr + tmp->p->size) == a)
    {
        //释放区与前空闲区相连
        tmp->p->size += size;
        //释放区与前后空闲区相连
        if (a + size == tmp->addr)
        {
            tmp->p->size += tmp->size;
            pre = tmp->p;
            next = tmp->n;
            next->p = pre;
            pre->n = next;
            if (tmp == current)
            {
                current = current->n;
            }
        }
        return a;
    }
    //释放区与后空闲区相连
    else if (a + size == tmp->addr)
    {
        tmp->addr -= size;
        tmp->size += size;
        return a;
    }
    //释放区与前后空闲区均不相连
    else
    {
        tmp->p->n = (struct map *)malloc(sizeof(struct map));
        tmp->p->n->addr = addr;
        tmp->p->n->size = size;
        tmp->p->n->p = tmp->p;
        tmp->p->n->n = tmp;
        tmp->p = tmp->p->n;
        return a;
    }
}

void display(struct map *display)
{
    int num;
    num = 0;
    struct map *tmp;
    tmp = display->n;
    printf("--------------------------------------------------------------------------\n");
    do
    {
        printf("| 空闲区编号: %u | 起始地址: %u | 结束地址: %u | 区域大小: %u |\n",
                num, tmp->addr, (tmp->addr) + (tmp->size), tmp->size);
        num++;
        tmp = tmp->n;
    } while (tmp != display->n);
    printf("--------------------------------------------------------------------------\n");
}

int main()
{
    char *memory = (char *)malloc(sizeof(char) * 1000);
    if (memory == NULL)
    {
        printf("分配失败！\n");
        exit(-1);
    }

    struct map *coremap = (struct map *)malloc(sizeof(struct map));
    coremap->addr = memory;
    coremap->size = 1000;
    coremap->n = coremap;
    coremap->p = coremap;
    printf("初始化成功。\n");
    printf("输入列表:\n");
    printf("1.输入“m 一个整数”来分配空间。\n");
    printf("2.输入“f 大小 地址”来释放空间。\n");
    printf("3.输入“d”显示空间使用情况。\n");
    printf("4.输入“e”退出程序。\n");

    char input;
    int size;
    char *addr;
    struct map *current, *search;
    current = coremap;
    search = coremap;
    display(coremap);
    do
    {
        input = getchar();
        if (input == 'm')
        {
            scanf(" %u", &size);
            current = lmalloc(current, size);
            display(coremap);
        }
        else if (input == 'f')
        {
            scanf(" %u %u", &size, &addr);
            search = coremap;
            lfree(search, size, addr);
            display(search);
        }
        else if (input == 'd')
        {
            display(coremap);
        }
        else if (input == 'e')
        {
            printf("再见！\n");
            break;
        }
        else if (input == '\n' || input == '\t' || input == ' ')
        {
            continue;
        }
        else
        {
            printf("非法输入！\n");
        }
    } while (true);
    free(memory);
    return 0;
}