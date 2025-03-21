#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

int syntax_error_count = 0;

static bool is_command_line(const char *line) {
    return line[0] == '\t';
}

static bool is_empty_line(const char *line) {
    while (*line) {
        if (!isspace((unsigned char)*line)) return false;
        line++;
    }
    return true;
}

void check(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("错误: 无法打开文件 '%s'\n", filename);
        syntax_error_count++;
        return;
    }

    char line[1024];
    bool in_rule = false; 
    int line_number = 0;  

    while (fgets(line, sizeof(line), file)) {
        line_number++;

        char *end = line + strlen(line) - 1;
        while (end >= line && isspace((unsigned char)*end)) {
            *end-- = '\0';
        }

        if (is_empty_line(line)) {
            in_rule = false; 
            continue;
        }
  
        if (!in_rule && strchr(line, '=')) {
            continue; 
        }

        // 检查是否是规则定义（以冒号结尾的行）
        char *colon = strchr(line, ':');
if (colon && !isspace((unsigned char)line[0])) {
    // 检查冒号前是否有空格
    if (isspace((unsigned char)*(colon - 1))) {
        printf("Line %d: 错误: 目标和依赖之间缺少冒号或存在多余空格 '%s'\n", line_number, line);
        syntax_error_count++;
    }
    
    in_rule = true;
} else if (in_rule) {
            // 如果在规则部分，检查命令行是否以制表符开头
            if (!is_command_line(line)) {
                if (strncmp(line, "    ", 4) == 0) {
                    printf("Line %d: 错误: 命令使用了 4 个空格代替制表符: '%s'\n", line_number, line);
                } 
                else if (line[0] == ' ' || line[0] == '\t') {
                    printf("Line %d: 错误: 命令行以混合空格和制表符开头: '%s'\n", line_number, line);
                } 
               
                else {
                    printf("Line %d: 错误: 命令行未以制表符开头: '%s'\n", line_number, line);
                }
                syntax_error_count++;
            } else {
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
            printf("Line %d: 错误: 缺少冒号的目标定义 '%s'\n", line_number, line);
            syntax_error_count++;
        }
    }

    fclose(file);
}