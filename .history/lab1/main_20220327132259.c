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
        printf("Malloc space size must be less than 1000!\n");
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
        printf("Failed to malloc because no enough free space\n");
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
        printf("Free space size must be less than 1000!\n");
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
            printf("The zone needn't freeing\n");
            return a;
        }
    }
    else if (((tmp->p->addr + tmp->p->size) > a) || (a + size > tmp->addr))
    {
        printf("The zone needn't freeing\n");
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
        printf("| free zone NO.: %u | start address: %u | end address: %u | size: %u |\n",
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
        printf("Failed to malloc！\n");
        exit(-1);
    }

    struct map *coremap = (struct map *)malloc(sizeof(struct map));
    coremap->addr = memory;
    coremap->size = 1000;
    coremap->n = coremap;
    coremap->p = coremap;
    printf("malloc successfuly,there are 1000 Bytes in total。\n");
    printf("command list:\n");
    printf("1.apply 'x' Bytes space: 'm x'.eg: 'm 100' \n");
    printf("2.free space starts from address and size is x : 'f x address'. eg:'f 100 17650'\n");
    printf("3.display space usage:'d'\n");
    printf("4.quit:'e'\n");

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
            printf("Illegal command\n");
        }
    } while (true);
    free(memory);
    return 0;
}