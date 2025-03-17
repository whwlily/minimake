#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include"basic.h"
#include"process.h"
#include"utils.h"   

int main(int argc, char *argv[]) {
    void recognize(int argc, char*argv[]);//调用 recognize.c 中的函数

    //
    bool verbose = false;

    // 检查命令行参数
    if (argc < 2) {
        printf("用法: minimake <目标> [-v]\n");
        printf("提示: 请提供一个目标，例如 'make run' 或 'make run-verbose'\n");
        return 1;
    }

    // 检查是否启用调试模式
    if (argc == 3 && strcmp(argv[2], "-v") == 0) {
        verbose = true;
    }

    const char *makefile = "Makefile"; // 默认的 Makefile 文件名
    const char *target = argv[1];      // 用户指定的目标

    // 解析 Makefile
    if (parse_makefile(makefile) != 0) {
        printf("错误: 无法解析 Makefile 文件 '%s'\n", makefile);
        return 1;
    }

    // 调试模式下打印变量和规则
    if (verbose) {
        debug_print_variables();
    }

    // 检查规则的有效性
    if (check_makefile() != 0) {
        printf("错误: Makefile 中存在无效的规则或依赖\n");
        return 1;
    }

    // 运行目标
    run_command(target);

    printf("Hello, World!\n");

    return 0;
}
