# 编译器
CC = gcc

# 编译选项
CFLAGS = -Wall -Werror -Wpedantic -std=c99

# 源文件
SRCS = main.c preprocess.c syntax-check.c rule-parse.c map.c line.c timecheck.c time.c system.c

# 目标文件
OBJS = main.o preprocess.o syntax-check.o rule-parse.o map.o line.o timecheck.o time.o system.o

# 可执行文件
TARGET = minimake

# 默认目标
all: minimake

# 生成 minimake 可执行文件
minimake: main.o preprocess.o syntax-check.o rule-parse.o map.o line.o timecheck.o time.o system.o variables.o
	gcc -Wall -Werror -Wpedantic -std=c99 -o minimake main.o preprocess.o syntax-check.o rule-parse.o map.o line.o timecheck.o time.o system.o variables.o

# 生成 main.o
main.o: main.c title.h map.h system.h time.h variables.h
	gcc -Wall -Werror -Wpedantic -std=c99 -c main.c -o main.o

# 生成 preprocess.o
preprocess.o: preprocess.c title.h
	gcc -Wall -Werror -Wpedantic -std=c99 -c preprocess.c -o preprocess.o

# 生成 syntax-check.o
syntax-check.o: syntax-check.c title.h
	gcc -Wall -Werror -Wpedantic -std=c99 -c syntax-check.c -o syntax-check.o

# 生成 rule-parse.o
rule-parse.o: rule-parse.c title.h map.h time.h
	gcc -Wall -Werror -Wpedantic -std=c99 -c rule-parse.c -o rule-parse.o

# 生成 map.o
map.o: map.c map.h
	gcc -Wall -Werror -Wpedantic -std=c99 -c map.c -o map.o

# 生成 line.o
line.o: line.c line.h title.h
	gcc -Wall -Werror -Wpedantic -std=c99 -c line.c -o line.o

#生成timecheck.o
timecheck.o: timecheck.c time.h
	gcc -Wall -Werror -Wpedantic -std=c99 -c timecheck.c -o timecheck.o

#生成time.o
time.o: time.c time.h
	gcc -Wall -Werror -Wpedantic -std=c99 -c time.c -o time.o

#生成system.o
system.o: system.c system.h
	gcc -Wall -Werror -Wpedantic -std=c99 -c system.c -o system.o

#生成variables.o
variables.o: variables.c variables.h
	gcc -Wall -Werror -Wpedantic -std=c99 -c variables.c -o variables.o

# 清理生成的文件
clean:
	rm -f main.o preprocess.o syntax-check.o rule-parse.o map.o minimake line.o timecheck.o system.o variables.o

# 测试目标 app
app: test.c utils.c
	gcc -o app test.c utils.c

# 测试目标 app1
app1: test.c utils.c
	gcc -o app1 test.c utils.c
app1: test.c utils.c
	gcc -o app1 test.c utils.c

# 测试目标 app2
app2: test.c utils.c missing.c
	gcc -o app2 test.c utils.c missing.c

# 测试目标 app3
app3: test.c utils.c lib
	gcc -o app3 test.c utils.c lib

cleanapp:
	rm -f app