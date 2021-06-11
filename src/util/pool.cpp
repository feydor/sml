#include "pool.h"

T Pool::request()
{
    for (size_t i = 0; i < POOL_SIZE; ++i) {
        if (!objects[i].in_use())
            return objects[i].init();
    }
}
