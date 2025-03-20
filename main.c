#include "title.h"
#include "map.h"
#include "line.h"
#include "timecheck.h"
#include "system.h"
#include "variables.h" // 包含变量处理模块
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// 定义语法检查函数的返回值
extern int syntax_error_count;

// 计算目标的入度
int calculate_indegree(const char *target) {
    int indegree = 0;
    for (int i = 0; i < rule_count; i++) {
        for (int j = 0; j < rules[i].dep_count; j++) {
            if (strcmp(rules[i].dependencies[j], target) == 0) {
                indegree++;
            }
        }
    }
    return indegree;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("用法: minimake <目标> [选项]\n");
        printf("运行 'minimake --help' 查看帮助信息。\n");
        return 1;
    }

    const char *target = NULL;
    bool verbose = false;
    bool run_all = false; // 标志：是否运行所有规则
    bool run_line = false; // 标志：是否运行 line 指令
    bool show_graph = false; // 标志：是否显示依赖关系图
    bool show_variables = false; // 标志：是否打印存储的变量

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
            printf("  --graph       显示依赖关系图\n");
            printf("  --line        调用拓扑排序函数\n");
            printf("  --variable    打印存储的变量\n");
            return 0;
        } else if (strcmp(argv[i], "--run") == 0) {
            run_all = true;
        } else if (strcmp(argv[i], "--graph") == 0) {
            show_graph = true; // 设置全局标志
        } else if (strcmp(argv[i], "--line") == 0) {
            run_line = true; // 设置 line 指令标志
        } else if (strcmp(argv[i], "--variable") == 0) {
            show_variables = true; // 设置打印变量标志
        } else if (target == NULL) {
            target = argv[i]; // 第一个非选项参数作为目标
        } else {
            printf("错误: 未知参数 '%s'\n", argv[i]);
            return 1;
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

    // 调用语法检查函数
    syntax_error_count = 0; // 初始化语法错误计数器
    check("./Minimake_claered.mk");
    if (syntax_error_count > 0) {
        printf("错误: Makefile 语法检查失败，共发现 %d 个错误。\n", syntax_error_count);
        return 1; // 如果语法检查失败或有警告，立即退出
    } else {
        printf("语法检查成功\n");
    }
    
    // 调用解析变量函数
    parse_makefile_variables("Makefile");

    // 解析规则
    parse_rules("Makefile");

    // 如果指定了 --variable 参数，则打印存储的变量
    if (show_variables) {
        printf("打印存储的变量:\n");
        print_variables(); // 调用打印变量的函数
        return 0;
    }

    // 如果指定了 --graph 参数，则显示依赖关系图
    if (show_graph) {
        printf("依赖关系图:\n");
        for (int i = 0; i < rule_count; i++) {
            // 如果指定了目标，则只打印该目标的依赖关系
            if (target != NULL && strcmp(rules[i].target, target) != 0) {
                continue;
            }

            // 跳过与 minimake、all 和 clean 相关的目标
            if (strcmp(rules[i].target, "minimake") == 0 ||
                strcmp(rules[i].target, "main.o") == 0 ||
                strcmp(rules[i].target, "preprocess.o") == 0 ||
                strcmp(rules[i].target, "syntax-check.o") == 0 ||
                strcmp(rules[i].target, "rule-parse.o") == 0 ||
                strcmp(rules[i].target, "map.o") == 0 ||
                strcmp(rules[i].target, "all") == 0 ||
                strcmp(rules[i].target, "clean") == 0) {
                continue;
            }

            // 计算入度
            int indegree = calculate_indegree(rules[i].target);

            // 打印目标及其依赖
            printf("目标: %s, 入度: %d, 依赖: ", rules[i].target, indegree);
            if (rules[i].dep_count > 0) {
                for (int j = 0; j < rules[i].dep_count; j++) {
                    printf("%s ", rules[i].dependencies[j]);
                }
            } else {
                printf("(无依赖)");
            }
            printf("\n");

            // 如果只需要打印指定目标，找到后直接退出循环
            if (target != NULL) {
                break;
            }
        }
        return 0;
    }

    // 如果指定了 --line 参数，则调用拓扑排序函数
    if (run_line) {
        printf("调用拓扑排序函数:\n");
        topological_sort(target); // 调用 line.c 中的拓扑排序函数
        printf("按顺序检查与构建目标:\n");
        timecheck(target);
        return 0;
    }

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

            // 使用 my_system 执行命令
            printf("[INFO] 调用 my_system 执行命令: %s\n", rules[i].commands);
            int result = my_system(rules[i].commands);
            if (result != 0) {
                printf("[ERROR] my_system 执行失败，命令: '%s'，返回值: %d\n", rules[i].commands, result);
                return 1;
            } else {
                printf("[SUCCESS] my_system 执行成功，命令: '%s'\n", rules[i].commands);
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

            // 使用 my_system 执行命令
            printf("[INFO] 调用 my_system 执行命令: %s\n", rules[i].commands);
            int result = my_system(rules[i].commands);
            if (result != 0) {
                printf("[ERROR] my_system 执行失败，命令: '%s'，返回值: %d\n", rules[i].commands, result);
                return 1;
            } else {
                printf("[SUCCESS] my_system 执行成功，命令: '%s'\n", rules[i].commands);
            }
            return 0;
        }
    }

    printf("错误: 未找到目标 '%s'\n", target);
    return 1;
}