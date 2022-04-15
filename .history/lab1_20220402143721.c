#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//Memory block data structure model
struct map{
    unsigned m_size;
    char *m_addr;
    struct map *next,*prior;
}

// struct map *lmalloc(struct map *cur, unsigned size)
// cur:last alloc location,size:size of needed memory
struct map *lmalloc(struct map *cur, unsigned size)
{
    char *a = cur->addr;
    struct map *pre, *next;
    struct map *tmp = cur;
    if ((size <= 0) || (size > 1000))
    {
        printf("Malloc space size must be less than 1000!\n");
        return cur;
    }
    if (cur->size >= size)  //当前区空间够大
    {
        cur->addr += size;
        cur->size -= size;
        if (cur->size == 0)
        {
            pre = cur->p;
            next = cur->n;
            next->p = pre;
            pre->n = next;
            cur = next;
            return cur->n;
        }   
    }
    else   //当前区的空间不够
    {
        tmp = tmp->n;
        while (tmp != cur)
        {
            if (tmp->size >= size)
            {
                cur = tmp;
                cur->addr += size;
                cur->size -= size;
                if (cur->size == 0)
                {
                    pre = cur->p;
                    next = cur->n;
                    next->p = pre;
                    pre->n = next;
                    cur = next;
                }
                return cur->n;
            }
            else tmp = tmp->n;
        }
        printf("Failed to malloc because no enough free space\n");
    }
    return cur->n;
}