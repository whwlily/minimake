#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// 处理 Makefile 的函数
void process_makefile(const char *filename, int verbose) {
    FILE *file = fopen(filename, "r"); // 打开 Makefile
    if (file == NULL) {
        perror("无法打开 Makefile");
        exit(EXIT_FAILURE);
    }

    FILE *output_file = NULL;
    if (verbose) {
        output_file = fopen("Minimake_cleared.mk", "w"); // 打开输出文件
        if (output_file == NULL) {
            perror("无法创建输出文件");
            fclose(file);
            exit(EXIT_FAILURE);
        }
    }

    char line[1024]; // 用于存储每一行的内容
    while (fgets(line, sizeof(line), file)) {
        // 去除行尾换行符
        line[strcspn(line, "\n")] = '\0';

        // 去除行尾空格
        int len = strlen(line);
        while (len > 0 && isspace(line[len - 1])) {
            line[--len] = '\0';
        }

        // 去除注释（# 后的内容）
        char *comment_start = strchr(line, '#');
        if (comment_start != NULL) {
            *comment_start = '\0'; // 截断注释部分
        }

        // 去除注释后的行尾空格
        len = strlen(line);
        while (len > 0 && isspace(line[len - 1])) {
            line[--len] = '\0';
        }

        // 过滤空行
        if (line[0] == '\0') {
            continue;
        }

        // 输出清理后的行
        if (verbose) {
            fprintf(output_file, "%s\n", line); // 写入输出文件
        }
        printf("%s\n", line); // 打印到控制台
    }

    // 关闭文件
    fclose(file);
    if (verbose) {
        fclose(output_file);
    }
}