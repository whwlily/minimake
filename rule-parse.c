#include "title.h"

#include <stdio.h>
#include <string.h>

struct Rule rules[MAX_RULES]; // 定义规则数组
int rule_count = 0;           // 定义规则计数器


// 检查文件是否存在
bool file_exists(const char *filename) {
    struct stat buffer;
    return (stat(filename, &buffer) == 0);
}

// 解析规则函数
void parse_rules(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("错误: 无法打开文件 '%s'\n", filename);
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        // 忽略空行或注释行
        if (line[0] == '\n' || line[0] == '#') {
            continue;
        }

        // 解析目标和依赖
        char *colon = strchr(line, ':');
        if (colon) {
            *colon = '\0';
            strncpy(rules[rule_count].target, line, sizeof(rules[rule_count].target) - 1);
            rules[rule_count].target[sizeof(rules[rule_count].target) - 1] = '\0';

            // 解析依赖
            char *dependency = strtok(colon + 1, " \t\n");
            rules[rule_count].dep_count = 0;
            while (dependency) {
                strncpy(rules[rule_count].dependencies[rules[rule_count].dep_count], dependency, 32);
                rules[rule_count].dep_count++;
                dependency = strtok(NULL, " \t\n");
            }

            // 读取命令
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
        // 如果指定了目标且当前目标不匹配，则跳过
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
        // 如果指定了目标且当前目标不匹配，则跳过
        if (target != NULL && strcmp(rules[i].target, target) != 0) {
            continue;
        }

        for (int j = 0; j < rules[i].dep_count; j++) {
            const char *dependency = rules[i].dependencies[j];
            bool valid = false;

            // 检查依赖是否是文件
            if (file_exists(dependency)) {
                valid = true;
            }

            // 检查依赖是否是其他目标
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

    return error_count > 0 ? 1 : 0; // 如果有错误，返回 1；否则返回 0
}