#ifndef LINE_H
#define LINE_H
#include"title.h"

// 声明计算入度函数
int calculate_indegree(const char *target);

// 声明拓扑排序函数
void topological_sort(const char *target);

#endif // LINE_H