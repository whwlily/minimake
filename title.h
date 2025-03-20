#ifndef TITLE_H
#define TITLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <ctype.h>

#define MAX_RULES 100
#define MAX_DEPENDENCIES 10

struct Rule {
    char target[128];
    char commands[128];
    int dep_count;
    char dependencies[MAX_DEPENDENCIES][32];
};

extern struct Rule rules[MAX_RULES]; // 声明规则数组
extern int rule_count;              // 声明规则计数器
// 声明全局变量 syntax_error_count
extern int syntax_error_count;


void parse_rules(const char *filename);
int validate_rules();

// 声明 file_exists 函数
bool file_exists(const char *filename);

int preprocess_makefile(const char *filename,bool verbose);
int check(const char *filename);


void print_utils_message();
#endif // TITLE_H
