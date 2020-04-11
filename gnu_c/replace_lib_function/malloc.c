#include <stdio.h>
#include <stdint.h>
#include <dlfcn.h>

void* malloc(size_t size)
{
    static void *(*real_malloc)(size_t) = NULL;
    if (real_malloc == NULL) {
        real_malloc = dlsym(RTLD_NEXT, "malloc");
    }
    printf("malloc called, size = %zu\n", size);
    return real_malloc(size);
}

int main(int argc, char const *argv[])
{
        malloc(10);
        return 0;
}