#include "title.h"
#include "map.h"
#include "line.h"
#include "timecheck.h"
#include "system.h"
#include "variables.h" 
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
    bool run_all = false; 
    bool run_line = false; 
    bool show_graph = false;
    bool show_variables = false; 
    bool check_time = false; 


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
            printf("  --timecheck   检查时间戳\n");
            printf("请先进行调试模式用来验证语法是否存在错误\n");
            return 0;
        } else if (strcmp(argv[i], "--run") == 0) {
            run_all = true;
        } else if (strcmp(argv[i], "--graph") == 0) {
            show_graph = true; 
        } else if (strcmp(argv[i], "--line") == 0) {
            run_line = true; 
        } else if (strcmp(argv[i], "--variable") == 0) {
            show_variables = true; 
        } else if (strcmp(argv[i], "--timecheck") == 0) {
            check_time = true; 
        } else if (target == NULL) {
            target = argv[i]; 
        } else {
            printf("错误: 未知参数 '%s'\n", argv[i]);
            return 1;
        }
    }
    if (verbose) {
        printf("调试模式已启用。\n");
    }

    //预处理
    if (preprocess_makefile("Makefile", verbose) != 0) {
        printf("错误: 预处理 Makefile 失败。\n");
        return 1;
    } else {
        printf("预处理成功\n");
    }

    // 语法检查
    syntax_error_count = 0; // 初始化语法错误计数器
    check("./Minimake_claered.mk");
    if (syntax_error_count > 0) {
        printf("错误: Makefile 语法检查失败，共发现 %d 个错误。\n", syntax_error_count);
        return 1; 
    } else {
        printf("语法检查成功\n");
    }
    
    // 解析变量
    parse_makefile_variables("Makefile");

    // 解析规则
    parse_rules("Makefile");

    
    if (show_variables) {
        printf("打印存储的变量:\n");
        print_variables(); 
        return 0;
    }

    
    if (show_graph) {
        printf("依赖关系图:\n");
        for (int i = 0; i < rule_count; i++) {
            
            if (target != NULL && strcmp(rules[i].target, target) != 0) {
                continue;
            }

            
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

            
            int indegree = calculate_indegree(rules[i].target);

            
            printf("目标: %s, 入度: %d, 依赖: ", rules[i].target, indegree);
            if (rules[i].dep_count > 0) {
                for (int j = 0; j < rules[i].dep_count; j++) {
                    printf("%s ", rules[i].dependencies[j]);
                }
            } else {
                printf("(无依赖)");
            }
            printf("\n");

            
            if (target != NULL) {
                break;
            }
        }
        return 0;
    }

    
    if (check_time) {
        if (target == NULL) {
            printf("错误: 未指定目标，无法检查时间戳。\n");
            return 1;
        }
        printf("检查目标 '%s' 的时间戳:\n", target);
        timecheck(target); 
    }

    
    if (run_line) {
        printf("调用拓扑排序函数:\n");
        topological_sort(target); 
        return 0;
    }

    
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

            
            if (strlen(rules[i].commands) == 0) {
                printf("错误: 目标 '%s' 没有有效的命令。\n", rules[i].target);
                return 1;
            }

           
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

    
    if (target == NULL) {
        printf("错误: 未指定目标。\n");
        return 1;
    }

    
    if (validate_rules(target) != 0) {
        printf("错误: Makefile 规则验证失败。\n");
        return 1;
    }

   
    for (int i = 0; i < rule_count; i++) {
        if (strcmp(rules[i].target, target) == 0) {
            printf("执行目标: %s\n", rules[i].target);
            if (verbose) {
                printf("执行命令: %s\n", rules[i].commands);
            }

            
            if (strlen(rules[i].commands) == 0) {
                printf("错误: 目标 '%s' 没有有效的命令。\n", rules[i].target);
                return 1;
            }

           
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