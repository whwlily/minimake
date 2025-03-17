# 编译器
CC = gcc

# 编译选项
CFLAGS = -Wall -Wextra -std=c99 -O2

# 源文件
SRCS = main.c process.c recognize.c utils.c

# 目标文件
OBJS = main.o process.o recognize.o utils.o

# 目标可执行文件
TARGET = myprogram

# 默认目标
all: myprogram

# 生成可执行文件
myprogram: main.o process.o recognize.o utils.o
	$(CC) $(CFLAGS) -o myprogram main.o process.o recognize.o utils.o

# 生成目标文件
main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o

process.o: process.c
	$(CC) $(CFLAGS) -c process.c -o process.o

recognize.o: recognize.c
	$(CC) $(CFLAGS) -c recognize.c -o recognize.o

utils.o: utils.c
	$(CC) $(CFLAGS) -c utils.c -o utils.o

# 清理生成的文件
clean:
	rm -f main.o process.o recognize.o utils.o myprogram

# 运行程序
run: myprogram
	./myprogram

# 运行程序并启用调试模式
run-verbose: myprogram
	./myprogram -v

# 显示帮助信息
help:
	@echo "可用目标:"
	@echo "  all          编译程序（默认目标）"
	@echo "  clean        清理生成的文件"
	@echo "  run          运行程序"
	@echo "  run-verbose  运行程序并启用调试模式"
	@echo "  help         显示帮助信息"

# 生成app可执行文件
app: main.c utils.c
	gcc -o app main.c utils.c