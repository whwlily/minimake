#define _GNU_SOURCE 
#include <string.h> 
#include <stdlib.h> 
#include <stdio.h>  
#include "variables.h"
#include "system.h" 

// 打印所有存储的变量
void print_variables() {
    printf("当前存储的变量:\n");
    extern char **environ; 
    for (char **env = environ; *env != NULL; env++) {
        if (strstr(*env, "CC=") == *env || strstr(*env, "CFLAGS=") == *env ||
            strstr(*env, "SRCS=") == *env || strstr(*env, "OBJS=") == *env ||
            strstr(*env, "TARGET=") == *env) {
            printf("%s\n", *env); 
        }
    }
    printf("变量打印完成。\n");
}

// 解析 Makefile 中的变量定义并存储到环境变量表
void parse_makefile_variables(const char *makefile_path) {
    FILE *file = fopen(makefile_path, "r");
    if (!file) {
        perror("无法打开 Makefile");
        exit(1);
    }

    char line[1024];
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '\n' || line[0] == '#') {
            continue;
        }

        char *equal_sign = strchr(line, '=');
        if (equal_sign) {
            *equal_sign = '\0';
            char *name = strtok(line, " \t");
            char *value = strtok(equal_sign + 1, "\n");
            if (name && value) {
                setenv(name, value, 1); 
            }
        }
    }

    fclose(file);
}

// 从环境变量表中获取
const char *get_variable(const char *name) {
    const char *value = getenv(name); 
    if (!value) {
        fprintf(stderr, "警告: 未定义的变量 '%s'\n", name);
    }
    return value;
}

// 从环境变量表中解析
char *expand_variables(const char *input) {
    char *result = strdup(input);
    char *start, *end;

    // 查找变量模式 $(VAR) 或 ${VAR}
    while ((start = strstr(result, "$(")) || (start = strstr(result, "${"))) {
        end = strchr(start, ')');
        if (!end) {
            end = strchr(start, '}');
        }
        if (!end) {
            fprintf(stderr, "错误: 未闭合的变量引用: %s\n", start);
            break;
        }

        // 提取变量名
        size_t var_len = end - start - 2;
        char *var_name = strndup(start + 2, var_len);

        // 获取变量值
        const char *var_value = get_variable(var_name);
        if (!var_value) {
            var_value = ""; 
        }

        // 替换变量
        size_t result_len = strlen(result) + strlen(var_value) - (var_len + 3);
        char *new_result = (char *)malloc(result_len + 1);
        snprintf(new_result, result_len + 1, "%.*s%s%s", (int)(start - result), result, var_value, end + 1);

        free(result);
        free(var_name);
        result = new_result;
    }

    return result;
}


// 执行命令并替换变量
void execute_command_with_variables(const char *command) {
    char *expanded_command = expand_variables(command);
    printf("[INFO] 执行命令: %s\n", expanded_command);
    int result = my_system(expanded_command);
    if (result != 0) {
        printf("[ERROR] 命令执行失败: %s\n", expanded_command);
    } else {
        printf("[SUCCESS] 命令执行成功: %s\n", expanded_command);
    }
    free(expanded_command);
}