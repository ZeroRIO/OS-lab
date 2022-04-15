#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct map{
    unsigned m_size;
    char *m_addr;
    struct map *next,*prior;
}