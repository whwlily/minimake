#include "title.h"
#include"map.h"
#include <stdio.h>
#include <string.h>
#include"line.h"
#include"time.h"

struct Rule rules[MAX_RULES]; // 定义规则数组
int rule_count = 0;           // 定义规则计数器


// 解析规则函数
void parse_rules(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("错误: 无法打开文件 '%s'\n", filename);
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '\n' || line[0] == '#') {
            continue;
        }

        // 解析目标和依赖
        char *colon = strchr(line, ':');
        if (colon) {
            *colon = '\0';
            char target[256];
            strncpy(target, line, sizeof(target) - 1);
            target[sizeof(target) - 1] = '\0';

            // 初始化规则
            strncpy(rules[rule_count].target, target, sizeof(rules[rule_count].target) - 1);
            rules[rule_count].target[sizeof(rules[rule_count].target) - 1] = '\0';
            rules[rule_count].dep_count = 0;

            // 解析依赖
            char *dependency = strtok(colon + 1, " \t\n");
            while (dependency) {
                if (rules[rule_count].dep_count < MAX_DEPENDENCIES) {
                    strncpy(rules[rule_count].dependencies[rules[rule_count].dep_count], dependency, sizeof(rules[rule_count].dependencies[rules[rule_count].dep_count]) - 1);
                    rules[rule_count].dependencies[rules[rule_count].dep_count][sizeof(rules[rule_count].dependencies[rules[rule_count].dep_count]) - 1] = '\0';
                    rules[rule_count].dep_count++;
                } else {
                    printf("警告: 目标 '%s' 的依赖数量超过限制 (%d)\n", target, MAX_DEPENDENCIES);
                }
                dependency = strtok(NULL, " \t\n");
            }

          
            if (fgets(line, sizeof(line), file)) {
                strncpy(rules[rule_count].commands, line, sizeof(rules[rule_count].commands) - 1);
                rules[rule_count].commands[sizeof(rules[rule_count].commands) - 1] = '\0';
            }

            rule_count++;
            if (rule_count >= MAX_RULES) {
                printf("警告: 超过最大规则数限制 (%d)\n", MAX_RULES);
                break;
            }
        }
    }

    fclose(file);
}
// 验证规则函数
int validate_rules(const char *target) {
    int error_count = 0; // 用于记录错误数量

    // 检查是否有重复的目标
    for (int i = 0; i < rule_count; i++) {
        if (target != NULL && strcmp(rules[i].target, target) != 0) {
            continue;
        }

        for (int j = i + 1; j < rule_count; j++) {
            if (strcmp(rules[i].target, rules[j].target) == 0) {
                printf("错误: Duplicate target definition '%s'\n", rules[i].target);
                error_count++;
            }
        }
    }

    // 检查依赖是否有效
    for (int i = 0; i < rule_count; i++) {
        if (target != NULL && strcmp(rules[i].target, target) != 0) {
            continue;
        }

        for (int j = 0; j < rules[i].dep_count; j++) {
            const char *dependency = rules[i].dependencies[j];
            bool valid = false;

          
            if (file_exists(dependency)) {
                valid = true;
            }

           
            for (int k = 0; k < rule_count; k++) {
                if (strcmp(rules[k].target, dependency) == 0) {
                    valid = true;
                    break;
                }
            }

            if (!valid) {
                printf("错误: Invalid dependency '%s'\n", dependency);
                error_count++;
            }
        }
    }

    return error_count > 0 ? 1 : 0; 
}