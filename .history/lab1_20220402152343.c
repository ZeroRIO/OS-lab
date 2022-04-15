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
struct map *lmalloc(struct map *cur, unsigned size){
    if ((size <= 0) || (size > 1000)){
        printf("malloc space size must be more than 0 and less than 1KB!\n");
        return cur;
    }
    //there is enough space to malloc
    if (cur->m_size >= size){
        cur->m_addr += size;
        cur->m_size -= size;
        //all space malloced,update list
        if (cur->m_size == 0){
            cur->prior->next = cur->next;
            cur->next->prior = cur->prior;
            cur = cur->next;
        }
        return cur;
    }
    //no enough space,traverse to find one
    else{
        struct map *tmp = cur;//tmp to judge traverse end
        cur = cur->next;
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