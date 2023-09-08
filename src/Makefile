CC = gcc
FLAGS = -Wall -Werror -Wextra
LIBS = -lcheck -lsubunit -lm -lgcov
TEST_FILES := $(wildcard tests/test_*.c)
FUNC_FILES_C = lib/*.c
FUNC_FILES_O = *.o
CHECKFLAGS = $(shell pkg-config --cflags --libs check)
GCOVFLAGS = -fprofile-arcs -ftest-coverage
LIBNAME = s21_string.a
GLFLAGS = --coverage

all: s21_string.a

peer: gcov_report gcov_html

build_object_files:
	@$(CC) $(FLAGS) $(GCOVFLAGS) $(GLFLAGS) -c $(FUNC_FILES_C)

build_library:
	@ar rcs $(LIBNAME) $(FUNC_FILES_O)

s21_string.a:
	@$(CC) $(FLAGS) -c $(FUNC_FILES_C)
	@make build_library
	@rm -rf *.o

test: s21_string.a $(TEST_FILES)
	@$(foreach test_file, $(TEST_FILES), \
		$(CC) $(test_file) -L. s21_string.a $(CHECKFLAGS) $(GLFLAGS) $(GCOVFLAGS) -o $(basename $(test_file));)
	@make start_tests

start_tests:
	@$(foreach test_file, $(TEST_FILES), \
		./$(basename $(test_file));) #  rm ./$(basename $(test_file));

gcov_report: build_object_files build_library test
	gcov $(FUNC_FILES_C)
	~/.local/bin/gcovr -r . --html --html-details -o s21_string.html

gcov_html: start_tests gcov_report
	xdg-open s21_string.html

clean:
	@rm -rf *.o
	@rm -rf *.gcno
	@rm -rf tests/*.gcno
	@rm -rf *.gcda
	@rm -rf tests/*.gcda
	@rm -rf tests/*.gcda
	@rm -rf *.a
	@rm -rf *.gcov
	@rm -rf *.info
	@rm -rf *.html
	@rm -rf *.css
	@rm -rf s21_*_test
	@$(foreach test_file, $(TEST_FILES), \
		rm ./$(basename $(test_file));)

rebuild:
	@make clean
	@make