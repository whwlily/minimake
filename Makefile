# Makefile 示例

# 编译器
CC = gcc

# 编译选项
CFLAGS = -Wall -Wextra -std=c99 -O2

# 目标可执行文件
TARGET = myprogram

# 源文件
SRCS = main.c process.c recognize.c

# 目标文件
OBJS = $(SRCS:.c=.o)

# 默认目标
all: $(TARGET)

# 生成可执行文件
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# 生成目标文件
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# 清理生成的文件
clean:
	rm -f $(OBJS) $(TARGET) Minimake_cleared.mk

# 运行程序
run: $(TARGET)
	./$(TARGET)

# 运行程序并启用调试模式
run-verbose: $(TARGET)
	./$(TARGET) -v

# 运行程序并指定 Makefile 路径
run-custom: $(TARGET)
	./$(TARGET) --file /path/to/custom_makefile -v

# 显示帮助信息
help:
	@echo "可用目标:"
	@echo "  all          编译程序（默认目标）"
	@echo "  clean        清理生成的文件"
	@echo "  run          运行程序"
	@echo "  run-verbose  运行程序并启用调试模式"
	@echo "  run-custom   运行程序并指定 Makefile 路径"
	@echo "  help         显示帮助信息"