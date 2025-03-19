#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

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
        return;
    }

    char line[1024];
    bool in_rule = false; // 标记是否在规则部分
    while (fgets(line, sizeof(line), file)) {
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
            // 检查变量定义中的特殊语法
            const char *equals = strchr(line, '=');
            if (equals && strchr(equals, ':') && strchr(equals, '$')) {
                // 变量定义中包含模式替换和变量引用，合法
                continue;
            }
            // 如果包含 ':' 但不符合变量定义的语法，报错
            if (strchr(line, ':')) {
                printf("错误: 无效语法 '%s'\n", line);
            }
            continue; // 跳过变量定义的进一步检查
        }

        // 检查是否是规则定义（以冒号结尾的行）
        if (strchr(line, ':') && !isspace((unsigned char)line[0])) {
            in_rule = true; // 进入规则部分
        } else if (in_rule) {
            // 如果在规则部分，检查命令行是否以制表符开头
            if (!is_command_line(line)) {
                printf("警告: 命令未以制表符开头: '%s'\n", line);
            }
        }
    }

    // 关闭文件
    fclose(file);
}