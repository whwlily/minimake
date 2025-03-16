#include <stdio.h>
#include <string.h>
#include"basic.h"


int main(int argc, char *argv[]) {
    // 如果没有提供任何参数
    if (argc == 1) {
        printf("错误: 未提供参数。使用 --help 查看帮助信息。\n");
        return 1;
    }

    // 遍历所有参数
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--help") == 0) {
            // 处理 --help 参数
            print_help();
            return 0;
        } else if (strcmp(argv[i], "--version") == 0) {
            // 处理 --version 参数
            print_version();
            return 0;
        } else if (strcmp(argv[i], "--custom") == 0) {
            // 处理 --custom 参数
            if (i + 1 < argc) { // 确保后面有参数值
                handle_custom_arg(argv[i + 1]);
                i++; // 跳过参数值
            } else {
                printf("错误: --custom 参数需要提供一个值。\n");
                return 1;
            }
        } else {
            // 处理未知参数
            printf("错误: 未知参数 '%s'。使用 --help 查看帮助信息。\n", argv[i]);
            return 1;
        }
    }
   
    return 0;
}