COMP = gcc
FLAGS = -Wall -Werror -Wextra

all: s21_grep

s21_grep:
	$(COMP) $(FLAGS) s21_grep.c -o s21_grep

test: s21_grep
	bash test_grep.sh

clang:
	cp ../../materials/linters/.clang-format ./
	clang-format -i *.c *.h
	rm .clang-format

style:
	cp ../../materials/linters/.clang-format ./
	clang-format -n *.c *.h
	rm .clang-format

verter:
	bash ../../materials/build/run.sh

clean:
	rm -rf s21_grep

rebuild:
	clean all
