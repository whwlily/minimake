#include<stdio.h>
#include<string.h>

    // 打印帮助信息
void print_help() {
    printf("用法: ./program [选项]\n");
    printf("选项:\n");
    printf("  --help        显示帮助信息\n");
    printf("  --version     显示版本信息\n");
    printf("  --custom <arg> 自定义参数\n");
}

// 打印版本信息
void print_version() {
    printf("程序版本: 1.0.0\n");
}

// 处理自定义参数
void handle_custom_arg(const char *arg) {
    printf("自定义参数值为: %s\n", arg);
}
  


