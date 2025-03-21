#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>


// 去除行尾空格
void trim_trailing_whitespace(char *line) {
    char *end = line + strlen(line) - 1;
    while (end >= line && isspace((unsigned char)*end)) {
        *end-- = '\0';
    }
}

// 检查是否是空行
bool is_empty_line(const char *line) {
    while (*line) {
        if (!isspace((unsigned char)*line)) return false;
        line++;
    }
    return true;
}

// 去除注释
void remove_comment(char *line) {
    char *comment = strchr(line, '#');
    if (comment) *comment = '\0';
}

void replace_leading_spaces_with_tab(char *line) {
    if (line[0] == ' ') {
        char *p = line;
        while (*p == ' ') p++; // 跳过所有前导空格
        if (*p != '\0') {
            memmove(line + 1, p, strlen(p) + 1); // 将内容向后移动
            line[0] = '\t'; // 替换为制表符
        }
    }
}

int preprocess_makefile(const char *filename, bool verbose) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("错误: 无法打开文件 '%s'\n", filename);
        return 1;
    }

    FILE *output = NULL;
    if (verbose) {
        output = fopen("Minimake_claered.mk", "w");
        if (!output) {
            printf("错误: 无法创建文件 'Minimake_claered.mk'\n");
            fclose(file);
            return 1;
        }
        printf("正在创建文件 'Minimake_claered.mk'...\n");
    }

    char line[1024];
    while (fgets(line, sizeof(line), file)) {
        // 去除行尾空格
        trim_trailing_whitespace(line);

        // 去除注释
        remove_comment(line);

        // 写入输出文件（包括空行）
        if (verbose && output) {
            fprintf(output, "%s\n", line);
        }
    }

    fclose(file);
    if (verbose && output) {
        fclose(output);
        printf("文件 'Minimake_claered.mk' 已成功生成并关闭。\n");
    }

    return 0;
}