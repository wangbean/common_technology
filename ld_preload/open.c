#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <dlfcn.h>
#include <stdarg.h>

typedef int (*orig_open_f_type)(const char *__file, int __oflag, ...); 

int open(const char *__file, int __oflag, ...)
{
    orig_open_f_type orig_open = NULL;
    orig_open = (orig_open_f_type)dlsym(RTLD_NEXT, "open");

    printf("call me by (pid = %d, euid = %d) file name = %s\n",getpid(), geteuid(), __file);
    va_list args;
    va_start(args, __oflag);
    mode_t mode = va_arg(args, int);
    va_end(args);
    
    return orig_open(__file, __oflag, mode);
}
