#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <unistd.h>
#include "utils.h"

#define MAX_TARGET_NAME 32
#define MAX_DEPENDENCIES 10
#define MAX_COMMAND_LENGTH 256
#define MAX_RULES 100
#define MAX_VARIABLES 100
#define MAX_VARIABLE_NAME 32
#define MAX_VARIABLE_VALUE 256

typedef struct {
    char target[MAX_TARGET_NAME];
    char dependencies[MAX_DEPENDENCIES][MAX_TARGET_NAME];
    int dependency_count;
    char command[MAX_COMMAND_LENGTH];
} Rule;

typedef struct {
    char name[MAX_VARIABLE_NAME];
    char value[MAX_VARIABLE_VALUE];
} Variable;

Rule rules[MAX_RULES];
int rule_count = 0;

Variable variables[MAX_VARIABLES];
int variable_count = 0;

// 函数声明
void set_variable(const char *name, const char *value);
const char *get_variable(const char *name);
void replace_variables_in_string(char *str);
int parse_makefile(const char *filename);
void print_rules();
void debug_print_variables();
int check_makefile();
void run_command(const char *target);
int check_dependency_validity(const char *dep);

// 设置变量
void set_variable(const char *name, const char *value) {
    if (!name || !value) {
        printf("错误: 变量名或值为空\n");
        return;
    }

    for (int i = 0; i < variable_count; i++) {
        if (strcmp(variables[i].name, name) == 0) {
            snprintf(variables[i].value, MAX_VARIABLE_VALUE, "%s", value);
            return;
        }
    }

    if (variable_count >= MAX_VARIABLES) {
        printf("错误: 超出变量存储限制\n");
        return;
    }

    snprintf(variables[variable_count].name, MAX_VARIABLE_NAME, "%s", name);
    snprintf(variables[variable_count].value, MAX_VARIABLE_VALUE, "%s", value);
    variable_count++;
}

// 获取变量值
const char *get_variable(const char *name) {
    if (!name) {
        return NULL;
    }

    for (int i = 0; i < variable_count; i++) {
        if (strcmp(variables[i].name, name) == 0) {
            return variables[i].value;
        }
    }
    return NULL;
}

// 替换字符串中的变量
void replace_variables_in_string(char *str) {
    if (!str) {
        return;
    }

    char buffer[MAX_COMMAND_LENGTH];
    buffer[0] = '\0';

    char *start = str;
    char *pos;

    while ((pos = strstr(start, "$(")) != NULL) {
        strncat(buffer, start, pos - start);

        char *end = strchr(pos, ')');
        if (!end) {
            printf("错误: 未闭合的变量引用 '%s'\n", pos);
            exit(EXIT_FAILURE);
        }

        char var_name[MAX_VARIABLE_NAME];
        snprintf(var_name, sizeof(var_name), "%.*s", (int)(end - pos - 2), pos + 2);

        const char *var_value = get_variable(var_name);
        if (var_value) {
            strncat(buffer, var_value, sizeof(buffer) - strlen(buffer) - 1);
        } else {
            printf("警告: 未定义的变量 '%s'\n", var_name);
        }

        start = end + 1;
    }

    strncat(buffer, start, sizeof(buffer) - strlen(buffer) - 1);
    snprintf(str, MAX_COMMAND_LENGTH, "%s", buffer);
}

// 解析 Makefile
int parse_makefile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("无法打开 Makefile");
        return -1; // 返回 -1 表示失败
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0';

        if (line[0] == '\0' || line[0] == '#') {
            continue;
        }

        if (strchr(line, '=') != NULL) {
            char *name = strtok(line, "=");
            char *value = strtok(NULL, "=");
            if (name && value) {
                while (isspace(*name)) name++;
                while (isspace(*value)) value++;
                set_variable(name, value);
            }
        } else if (strchr(line, ':') != NULL) {
            char *target = strtok(line, ":");
            char *dependencies = strtok(NULL, ":");

            snprintf(rules[rule_count].target, MAX_TARGET_NAME, "%s", target);

            if (dependencies) {
                char *dep = strtok(dependencies, " ");
                while (dep) {
                    snprintf(rules[rule_count].dependencies[rules[rule_count].dependency_count], 
                             MAX_TARGET_NAME, "%s", dep);
                    rules[rule_count].dependency_count++;
                    dep = strtok(NULL, " ");
                }
            }

            if (fgets(line, sizeof(line), file)) {
                line[strcspn(line, "\n")] = '\0';
                replace_variables_in_string(line);
                snprintf(rules[rule_count].command, MAX_COMMAND_LENGTH, "%s", line);
            }

            rule_count++;
        }
    }

    fclose(file);
    return 0; // 返回 0 表示成功
}

// 打印规则
void print_rules() {
    for (int i = 0; i < rule_count; i++) {
        printf("目标: %s\n", rules[i].target);
        printf("依赖: ");
        for (int j = 0; j < rules[i].dependency_count; j++) {
            printf("%s ", rules[i].dependencies[j]);
        }
        printf("\n命令: %s\n", rules[i].command);
    }
}

// 调试打印变量
void debug_print_variables() {
    printf("已解析的变量:\n");
    for (int i = 0; i < variable_count; i++) {
        printf("  %s = %s\n", variables[i].name, variables[i].value);
    }
    printf("\n已解析的规则:\n");
    print_rules();
}

// 检查 Makefile
int check_makefile() {
    for (int i = 0; i < rule_count; i++) {
        for (int j = 0; j < rules[i].dependency_count; j++) {
            if (!check_dependency_validity(rules[i].dependencies[j])) {
                printf("错误: 无效的依赖 '%s'\n", rules[i].dependencies[j]);
                return -1;
            }
        }
    }
    return 0;
}

// 检查依赖的有效性
int check_dependency_validity(const char *dep) {
    if (!dep) {
        return 0;
    }

    // 检查是否是已定义的目标
    for (int i = 0; i < rule_count; i++) {
        if (strcmp(rules[i].target, dep) == 0) {
            return 1; // 依赖是已定义的目标
        }
    }

    // 检查是否是存在的文件
    if (access(dep, F_OK) == 0) {
        return 1; // 依赖是存在的文件
    }

    return 0; // 依赖无效
}

// 运行命令
void run_command(const char *target) {
    if (!target) {
        printf("错误: 目标为空\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < rule_count; i++) {
        if (strcmp(rules[i].target, target) == 0) {
            if (strlen(rules[i].command) == 0) {
                printf("错误: 目标 '%s' 没有命令可执行\n", target);
                exit(EXIT_FAILURE);
            }

            printf("运行命令: %s\n", rules[i].command);
            int ret = system(rules[i].command);
            if (ret != 0) {
                printf("错误: 命令 '%s' 执行失败，返回值: %d\n", rules[i].command, ret);
                exit(EXIT_FAILURE);
            }
            return;
        }
    }

    printf("错误: 未找到目标 '%s'\n", target);
    exit(EXIT_FAILURE);
}

#include <stdio.h>

void print_message() {
    printf("This is a utility function.\n");
}