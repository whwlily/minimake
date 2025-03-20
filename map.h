#ifndef MAP_H
#define MAP_H

#include <stdbool.h>

extern bool show_graph; // 声明全局变量

void add_dependency(const char *target, const char *dependency);
void print_graph();

#endif // MAP_H