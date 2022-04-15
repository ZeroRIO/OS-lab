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
            return cur;
        }
        return cur->next;
    }
    //no enough space,traverse to find one
    else{
        struct map *tmp = cur;//tmp to judge traverse end
        cur = cur->next;
        while (cur != tmp)
        {
            if (cur->m_size >= size)
            {
                cur->m_mddr += size;
                cur->m_size -= size;
                if (cur->m_size == 0){
                    cur->prior->next = cur->next;
                    cur->next->prior = cur->prior;
                    cur = cur->next;
                    return cur;
                }
                return cur->next;
            }
            else cur = cur->next;
        }
        printf("Failed to malloc because no matching free space\n");
    }
    return cur->next;
}
//char *lfree(struct map *cur, unsigned size, char *addr)
char *lfree(struct map *cur, unsigned size, char *addr){
    struct map *tmp = cur;
    struct map *next, *pre;
    char *a = addr;
    if ((size <= 0) || (size > 1000)){
        printf("Free space size must be more than 0 and less than 1KB!\n");
        return a;
    }
    
    do{
        tmp = tmp->next;
        if (((tmp->prior->m_addr <= a) && (tmp->m_addr >= a)) || (tmp->prior == tmp)
            || ((tmp->prior->m_addr > a) && (tmp->prior->prior->m_addr >= tmp->prior->m_addr)))
            break;
    } while (tmp != cur);

    if (tmp->prior == tmp){
        if ((tmp->m_addr <= a) && (tmp->m_addr + tmp->size >= a)){
            printf("The zone needn't free\n");
            return a;
        }
    }
    else if (((tmp->prior->m_addr + tmp->prior->size) > a) || (a + size > tmp->m_addr)){
        printf("The zone needn't freeing\n");
        return a;
    }

    //there is free zone front
    if ((tmp->prior->m_addr + tmp->prior->size) == a)
    {
        tmp->prior->size += size;
        //and there is free zone behind
        if (a + size == tmp->m_addr)
        {
            tmp->prior->size += tmp->size;
            tmp->prior->next = tmp->next;
            tmp->next->prior = tmp->prior;
            cur = tmp->next;
        }
        return a;
    }
    //there is free zone behind
    else if (a + size == tmp->m_addr)
    {
        tmp->m_addr -= size;
        tmp->size += size;
        return a;
    }
    //释放区与前后空闲区均不相连
    else
    {
        tmp->p->n = (struct map *)malloc(sizeof(struct map));
        tmp->p->n->m_addr = m_addr;
        tmp->p->n->size = size;
        tmp->p->n->p = tmp->p;
        tmp->p->n->n = tmp;
        tmp->p = tmp->p->n;
        return a;
    }
}