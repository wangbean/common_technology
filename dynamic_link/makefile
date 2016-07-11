all: main libtest_1.so libtest_2.so

main: main.c
	gcc -rdynamic -o main main.c -ldl
libtest_1.so: libtest_1.c
	gcc -fPIC -shared libtest_1.c -o libtest_1.so
libtest_2.so: libtest_2.c
	gcc -fPIC -shared libtest_2.c -o libtest_2.so
clean:
	rm -f libtest_2.so libtest_1.so main
