#include "title.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("用法: minimake <目标> [选项]\n");
        printf("运行 'minimake --help' 查看帮助信息。\n");
        return 1;
    }

    bool verbose = false;
    bool run_all = false; // 标志：是否运行所有规则
    const char *target = NULL;

    // 检查命令行参数
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0) {
            verbose = true;
        } else if (strcmp(argv[i], "--help") == 0) {
            printf("用法: minimake <目标> [选项]\n");
            printf("选项:\n");
            printf("  --help        显示帮助信息\n");
            printf("  -v, --verbose 启用调试模式\n");
            printf("  --run         运行所有规则\n");
            return 0;
        } else if (strcmp(argv[i], "--run") == 0) {
            run_all = true;
        } else {
            target = argv[i]; // 第一个非选项参数作为目标
        }
    }

    if (verbose) {
        printf("调试模式已启用。\n");
    }

    // 调用预处理函数
    if (preprocess_makefile("Makefile", verbose) != 0) {
        printf("错误: 预处理 Makefile 失败。\n");
        return 1;
    } else {
        printf("预处理成功\n");
    }

    if (check("./Minimake_claered.mk") != 0) {
        printf("错误: Makefile 语法检查失败。\n");
        return 1;
    } else {
        printf("语法检查成功\n");
    }

    // 解析规则
    parse_rules("Makefile");

    // 如果指定了 --run 参数，则运行所有规则
    if (run_all) {
        printf("运行所有规则...\n");

        // 验证所有规则
        if (validate_rules(NULL) != 0) {
            printf("错误: Makefile 规则验证失败。\n");
            return 1;
        }

        // 逐个运行所有规则
        for (int i = 0; i < rule_count; i++) {
            printf("执行目标: %s\n", rules[i].target);
            if (verbose) {
                printf("执行命令: %s\n", rules[i].commands);
            }

            // 检查命令是否为空
            if (strlen(rules[i].commands) == 0) {
                printf("错误: 目标 '%s' 没有有效的命令。\n", rules[i].target);
                return 1;
            }

            // 执行命令
            if (system(rules[i].commands) != 0) {
                printf("错误: 执行命令失败 '%s'\n", rules[i].commands);
                return 1;
            }
        }

        printf("所有规则执行完成。\n");
        return 0;
    }

    // 如果未指定目标，则提示错误
    if (target == NULL) {
        printf("错误: 未指定目标。\n");
        return 1;
    }

    // 验证指定目标的规则
    if (validate_rules(target) != 0) {
        printf("错误: Makefile 规则验证失败。\n");
        return 1;
    }

    // 查找并执行目标
    for (int i = 0; i < rule_count; i++) {
        if (strcmp(rules[i].target, target) == 0) {
            printf("执行目标: %s\n", rules[i].target);
            if (verbose) {
                printf("执行命令: %s\n", rules[i].commands);
            }

            // 检查命令是否为空
            if (strlen(rules[i].commands) == 0) {
                printf("错误: 目标 '%s' 没有有效的命令。\n", rules[i].target);
                return 1;
            }

            // 执行命令
            if (system(rules[i].commands) != 0) {
                printf("错误: 执行命令失败 '%s'\n", rules[i].commands);
                return 1;
            }
            return 0;
        }
    }

    printf("错误: 未找到目标 '%s'\n", target);
    return 1;
}