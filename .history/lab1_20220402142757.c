#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct map{
    unsigned m_size;
    char *m_addr;
    struct map *next,*prior;
}


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