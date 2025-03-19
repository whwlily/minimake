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

int preprocess_makefile(bool verbose) {
    const char *filename = "./Makefile";
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
    }

    char line[1024];
    bool in_rule = false; // 标记是否在规则部分
    while (fgets(line, sizeof(line), file)) {
        // 去除行尾空格
        trim_trailing_whitespace(line);

        // 跳过空行
        if (is_empty_line(line)) {
            in_rule = false; // 空行结束规则部分
            continue;
        }

        // 去除注释
        remove_comment(line);

        // 再次去除行尾空格
        trim_trailing_whitespace(line);

        // 跳过清理后的空行
        if (is_empty_line(line)) {
            in_rule = false; // 空行结束规则部分
            continue;
        }

        // 检查是否是规则定义（以冒号结尾的行）
        if (strchr(line, ':') && !isspace((unsigned char)line[0])) {
            in_rule = true; // 进入规则部分
        } else if (in_rule) {
            // 如果在规则部分，处理命令行
            replace_leading_spaces_with_tab(line);
        }

        // 在调试模式下输出清理后的行到 Minimake_claered.mk
        if (verbose && output) {
            fprintf(output, "%s\n", line);
        }
    }

    // 关闭文件
    fclose(file);
    if (verbose && output) {
        fclose(output);
    }

    return 0; // 确保函数返回值
}