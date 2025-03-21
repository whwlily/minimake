#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

// 定义全局变量 syntax_error_count
int syntax_error_count = 0;

// 检查行是否以制表符开头
static bool is_command_line(const char *line) {
    return line[0] == '\t';
}

// 检查是否是空行
static bool is_empty_line(const char *line) {
    while (*line) {
        if (!isspace((unsigned char)*line)) return false;
        line++;
    }
    return true;
}

// 检查 Minimake_claered.mk 文件的语法
void check(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("错误: 无法打开文件 '%s'\n", filename);
        syntax_error_count++;
        return;
    }

    char line[1024];
    bool in_rule = false; // 标记是否在规则部分
    int line_number = 0;  // 当前行号

    while (fgets(line, sizeof(line), file)) {
        line_number++;

        // 去除行尾空格
        char *end = line + strlen(line) - 1;
        while (end >= line && isspace((unsigned char)*end)) {
            *end-- = '\0';
        }

        // 跳过空行
        if (is_empty_line(line)) {
            in_rule = false; // 空行结束规则部分
            continue;
        }

        // 检查是否是变量定义（包含 '=' 且不在规则部分）
        if (!in_rule && strchr(line, '=')) {
            continue; // 跳过变量定义的进一步检查
        }

        // 检查是否是规则定义（以冒号结尾的行）
        char *colon = strchr(line, ':');
if (colon && !isspace((unsigned char)line[0])) {
    // 检查冒号前是否有空格
    if (isspace((unsigned char)*(colon - 1))) {
        printf("Line %d: 错误: 目标和依赖之间缺少冒号或存在多余空格 '%s'\n", line_number, line);
        syntax_error_count++;
    }
    // 移除对冒号后多余空格的警告
    in_rule = true; // 进入规则部分
} else if (in_rule) {
            // 如果在规则部分，检查命令行是否以制表符开头
            if (!is_command_line(line)) {
                // 检查是否使用了 4 个空格代替制表符
                if (strncmp(line, "    ", 4) == 0) {
                    printf("Line %d: 错误: 命令使用了 4 个空格代替制表符: '%s'\n", line_number, line);
                } 
                // 检查是否以其他错误的缩进开头
                else if (line[0] == ' ' || line[0] == '\t') {
                    printf("Line %d: 错误: 命令行以混合空格和制表符开头: '%s'\n", line_number, line);
                } 
                // 检查是否完全未以制表符开头
                else {
                    printf("Line %d: 错误: 命令行未以制表符开头: '%s'\n", line_number, line);
                }
                syntax_error_count++;
            } else {
                // 检查制表符后是否有多余的空格
                int i = 1;
                while (line[i] == ' ') {
                    i++;
                }
                if (i > 1) {
                    printf("Line %d: 错误: 命令行中制表符后存在多余空格: '%s'\n", line_number, line);
                    syntax_error_count++;
                }
            }
        } else if (colon == NULL) {
            // 如果没有冒号且不在规则部分，报错
            printf("Line %d: 错误: 缺少冒号的目标定义 '%s'\n", line_number, line);
            syntax_error_count++;
        }
    }

    // 关闭文件
    fclose(file);
}