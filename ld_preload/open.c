#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <dlfcn.h>
#include <stdarg.h>

typedef int (*orig_open_f_type)(const char *__file, int __oflag, ...); 

/* 带有"构造函数"属性的函数将在main()函数之前被执行，
 * 而声明为"析构函数"属性的函数则将在main()退出时执行。
 * 全局类的构造过程发生在"__attach"函数之前,而析构也发生在__detach()前面。
 */
/* 修改 /etc/ld.so.preload */
void __attribute__ ((constructor)) __attach(void);
void __attribute__ ((destructor)) __detach(void);

void __attach(void)
{
    return;
    
}

void __detach(void)
{
    return;
    
}

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
