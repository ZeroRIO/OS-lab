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
    char *a = cur->m_addr;
    struct map *pre, *next;
    struct map *tmp = cur;

    if ((size <= 0) || (size > 1000))
    {
        printf("malloc space size must be more than 0 and less than 1KB!\n");
        return cur;
    }

    if (cur->m_size >= size)  //当前区空间够大
    {
        cur->m_addr += size;
        cur->m_size -= size;
        if (cur->size == 0)
        {
            pre = cur->prior;
            next = cur->next;
            next->prior = pre;
            pre->next = next;
            cur = next;
            return cur->next;
        }   
    }
    else   //当前区的空间不够
    {
        tmp = tmp->next;
        while (tmp != cur)
        {
            if (tmp->m_size >= size)
            {
                cur = tmp;
                cur->m_addr += size;
                cur->m_size -= size;
                if (cur->m_size == 0)
                {
                    pre = cur->prior;
                    next = cur->next;
                    next->prior = pre;
                    pre->next = next;
                    cur = next;
                }
                return cur->next;
            }
            else tmp = tmp->next;
        }
        printf("Failed to malloc because no enough free space\n");
    }
    return cur->next;
}