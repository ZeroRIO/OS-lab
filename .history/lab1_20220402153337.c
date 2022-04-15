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
//char *lfree(struct map *cur, unsigned size, char *m_addr)
char *lfree(struct map *cur, unsigned size, char *m_addr){
    struct map *tmp = cur;
    struct map *next, *pre;
    char *a = m_addr;
    if ((size <= 0) || (size > 1000))
    {
        printf("Free space size must be more than 0 and less than 1KB!\n");
        return a;
    }
    do
    {
        tmp = tmp->n;
        if (((tmp->p->m_addr <= a) && (tmp->m_addr >= a)) || (tmp->p == tmp)
            || ((tmp->p->m_addr > a) && (tmp->p->p->m_addr >= tmp->p->m_addr)))
            break;
    } while (tmp != cur);

    if (tmp->p == tmp)
    {
        if ((tmp->m_addr <= a) && (tmp->m_addr + tmp->size >= a))
        {
            printf("The zone needn't freeing\n");
            return a;
        }
    }
    else if (((tmp->p->m_addr + tmp->p->size) > a) || (a + size > tmp->m_addr))
    {
        printf("The zone needn't freeing\n");
        return a;
    }

    if ((tmp->p->m_addr + tmp->p->size) == a)
    {
        //释放区与前空闲区相连
        tmp->p->size += size;
        //释放区与前后空闲区相连
        if (a + size == tmp->m_addr)
        {
            tmp->p->size += tmp->size;
            pre = tmp->p;
            next = tmp->n;
            next->p = pre;
            pre->n = next;
            if (tmp == cur)
            {
                cur = cur->n;
            }
        }
        return a;
    }
    //释放区与后空闲区相连
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