CC = gcc

# 定义编译选项
CFLAGS = -Wall -Wextra -std=c99

# 定义目标可执行文件
TARGET = myprogram

# 定义源文件
SRCS = main.c basic.c 

# 定义目标文件
OBJS = $(SRCS:.c=.o)

# 默认目标：编译并生成可执行文件
all: $(TARGET)

# 生成可执行文件
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# 生成目标文件
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# 清理生成的文件
clean:
	rm -f $(OBJS) $(TARGET)

# 运行程序
run: $(TARGET)
	./$(TARGET) --help

# 显示帮助信息
help:
	@echo "用法: make [目标]"
	@echo "目标:"
	@echo "  all    编译并生成可执行文件（默认）"
	@echo "  clean  清理生成的文件"
	@echo "  run    运行程序"
	@echo "  help   显示帮助信息"