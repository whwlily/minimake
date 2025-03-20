CC = gcc
CFLAGS = -Wall -Werror -Wpedantic -std=c99
SRCS = main.c preprocess.c syntax-check.c rule-parse.c map.c line.c timecheck.c time.c system.c
OBJS = main.o preprocess.o syntax-check.o rule-parse.o map.o line.o timecheck.o time.o system.o
TARGET = minimake
all: minimake
minimake: main.o preprocess.o syntax-check.o rule-parse.o map.o line.o timecheck.o time.o system.o variables.o
	gcc -Wall -Werror -Wpedantic -std=c99 -o minimake main.o preprocess.o syntax-check.o rule-parse.o map.o line.o timecheck.o time.o system.o variables.o
main.o: main.c title.h map.h system.h time.h variables.h
	gcc -Wall -Werror -Wpedantic -std=c99 -c main.c -o main.o
preprocess.o: preprocess.c title.h
	gcc -Wall -Werror -Wpedantic -std=c99 -c preprocess.c -o preprocess.o
syntax-check.o: syntax-check.c title.h
	gcc -Wall -Werror -Wpedantic -std=c99 -c syntax-check.c -o syntax-check.o
rule-parse.o: rule-parse.c title.h map.h time.h
	gcc -Wall -Werror -Wpedantic -std=c99 -c rule-parse.c -o rule-parse.o
map.o: map.c map.h
	gcc -Wall -Werror -Wpedantic -std=c99 -c map.c -o map.o
line.o: line.c line.h title.h
	gcc -Wall -Werror -Wpedantic -std=c99 -c line.c -o line.o
timecheck.o: timecheck.c time.h
	gcc -Wall -Werror -Wpedantic -std=c99 -c timecheck.c -o timecheck.o
time.o: time.c time.h
	gcc -Wall -Werror -Wpedantic -std=c99 -c time.c -o time.o
system.o: system.c system.h
	gcc -Wall -Werror -Wpedantic -std=c99 -c system.c -o system.o
variables.o: variables.c variables.h
	gcc -Wall -Werror -Wpedantic -std=c99 -c variables.c -o variables.o
clean:
	rm -f main.o preprocess.o syntax-check.o rule-parse.o map.o minimake line.o timecheck.o system.o variables.o
app: test.c utils.c
	gcc -o app test.c utils.c
app1: test.c utils.c
	gcc -o app1 test.c utils.c
app1: test.c utils.c
	gcc -o app1 test.c utils.c
app2: test.c utils.c missing.c
	gcc -o app2 test.c utils.c missing.c
app3: test.c utils.c lib
	gcc -o app3 test.c utils.c lib
cleanapp:
	rm -f app
