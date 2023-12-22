COMP = gcc
FLAGS = -Wall -Werror -Wextra

all:s21_cat

s21_cat:
	$(COMP) $(FLAGS) s21_cat.c -o s21_cat
test:
	bash test_func_cat.sh
style:
	cp ../../materials/linters/.clang-format ./
	clang-format -i *.c
	rm .clang-format
valgrind:
	valgrind --tool=memcheck --leak-check=yes  ./s21_cat
leaks:
	leaks -atExit -- ./s21_cat
clean:
	rm -rf s21_cat , *.o
rebuild:
	clean all