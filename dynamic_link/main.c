#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

typedef void* (*callback_t)(void*);

int
main(int argc, char **argv)
{
    void *handle;
    /* 打开动态库链接 */
    handle = dlopen(argv[1], RTLD_LAZY);
    if(!handle) {
        perror("dlopen failed...");
        exit(-1);
    }
    /* 获取一个函数 */
    callback_t call_func = (callback_t)dlsym(handle, "worker");
    call_func(NULL);
    dlclose(handle);
    return 0;
}
