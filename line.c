#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "line.h"
#include "map.h"

// 打印目标的直接依赖项
void print_direct_dependencies(const char *target) {
    for (int i = 0; i < rule_count; i++) {
        if (strcmp(rules[i].target, target) == 0) {
            printf("目标: %s 依赖: ", target);
            if (rules[i].dep_count > 0) {
                for (int j = 0; j < rules[i].dep_count; j++) {
                    printf("%s ", rules[i].dependencies[j]);
                }
            } else {
                printf("无");
            }
            printf("\n");
            break;
        }
    }
}

// 打印目标的所有依赖项（递归调用直接依赖打印函数）
void print_all_dependencies(const char *target) {
    for (int i = 0; i < rule_count; i++) {
        if (strcmp(rules[i].target, target) == 0) {
            // 打印当前目标的直接依赖项
            print_direct_dependencies(target);

            // 递归打印每个直接依赖项的依赖
            if (rules[i].dep_count > 0) {
                for (int j = 0; j < rules[i].dep_count; j++) {
                    print_all_dependencies(rules[i].dependencies[j]);
                }
            }
            break;
        }
    }
}

// 拓扑排序函数
void topological_sort(const char *target) {
    int *indegree = (int *)malloc(rule_count * sizeof(int));
    if (indegree == NULL) {
        printf("错误: 内存分配失败。\n");
        return;
    }

    // 初始化入度数组
    for (int i = 0; i < rule_count; i++) {
        indegree[i] = calculate_indegree(rules[i].target);
    }

    // 创建队列存储入度为 0 的目标
    char **queue = (char **)malloc(rule_count * sizeof(char *));
    if (queue == NULL) {
        printf("错误: 内存分配失败。\n");
        free(indegree);
        return;
    }
    int front = 0, rear = 0;

    // 将入度为 0 的目标加入队列
    for (int i = 0; i < rule_count; i++) {
        if (indegree[i] == 0) {
            // 如果目标未被显式指定，则跳过
            if (target != NULL && strcmp(rules[i].target, target) != 0) {
                continue;
            }
            queue[rear++] = rules[i].target;
        }
    }

    printf("拓扑排序结果:\n");
    while (front < rear) {
        // 从队列中取出一个目标
        const char *current = queue[front++];
        // 打印当前目标的所有依赖项（包括间接依赖）
        print_all_dependencies(current);

        // 遍历规则，更新依赖目标的入度
        for (int i = 0; i < rule_count; i++) {
            for (int j = 0; j < rules[i].dep_count; j++) {
                if (strcmp(rules[i].dependencies[j], current) == 0) {
                    indegree[i]--;
                    if (indegree[i] == 0) {
                        // 如果目标未被显式指定，则跳过
                        if (target != NULL && strcmp(rules[i].target, target) != 0) {
                            continue;
                        }
                        queue[rear++] = rules[i].target;
                    }
                }
            }
        }
    }

    free(indegree);
    free(queue);
}