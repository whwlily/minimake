#include <stdio.h>
#include <string.h>
#include"basic.h"
#include"process.h"

void recognize(int argc, char *argv[]) {
    int verbose = 0; // 是否启用调试模式
    const char *filename = "./Makefile"; // 默认文件路径

    // 检查命令行参数
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0) {
            verbose = 1; // 启用调试模式
        } else if (strcmp(argv[i], "--help") == 0) {
            // 处理 --help 参数
            printf("用法: ./myprogram [选项]\n");
            printf("选项:\n");
            printf("  --help        显示帮助信息\n");
            printf("  --version     显示版本信息\n");
            printf("  -v, --verbose 启用调试模式，输出清理后的 Makefile\n");
            printf("  --file <路径> 指定 Makefile 文件路径\n");
            return;
        } else if (strcmp(argv[i], "--version") == 0) {
            // 处理 --version 参数
            printf("版本信息: 1.0.0\n");
            return;
        } else if (strcmp(argv[i], "--file") == 0) {
            // 处理 --file 参数
            if (i + 1 < argc) {
                filename = argv[i + 1]; // 获取文件路径
                i++; // 跳过参数值
            } else {
                printf("错误: --file 参数需要提供一个文件路径。\n");
                return;
            }
        } else {
            // 处理未知参数
            printf("错误: 未知参数 '%s'。使用 --help 查看帮助信息。\n", argv[i]);
            return;
        }
    }

    // 处理 Makefile
    process_makefile(filename, verbose);
}