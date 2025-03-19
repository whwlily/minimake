CC = gcc
CFLAGS = -Wall -Werror -Wpedantic -std=c99
SRCS = main.c preprocess.c syntax-check.c rule-parse.c
OBJS = main.o preprocess.o syntax-check.o rule-parse.o
TARGET = minimake
all: minimake
minimake: main.o preprocess.o syntax-check.o rule-parse.o
	gcc -Wall -Werror -Wpedantic -std=c99 -o minimake main.o preprocess.o syntax-check.o rule-parse.o
main.o: main.c title.h
	gcc -Wall -Werror -Wpedantic -std=c99 -c main.c -o main.o
preprocess.o: preprocess.c title.h
	gcc -Wall -Werror -Wpedantic -std=c99 -c preprocess.c -o preprocess.o
syntax-check.o: syntax-check.c title.h
	gcc -Wall -Werror -Wpedantic -std=c99 -c syntax-check.c -o syntax-check.o
rule-parse.o: rule-parse.c title.h
	gcc -Wall -Werror -Wpedantic -std=c99 -c rule-parse.c -o rule-parse.o
clean:
	rm -f main.o preprocess.o syntax-check.o rule-parse.o minimake
app: test.c utils.c
	gcc -o app test.c utils.c
app1: test.c utils.c
	gcc -o app test.c utils.c
app1: test.c utils.c
	gcc -o app test.c utils.c
app2: test.c utils.c missing.c
	gcc -o app test.c utils.c missing.c
app3: test.c utils.c lib
	gcc -o app test.c utils.c lib
