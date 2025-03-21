
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <stdbool.h> 


#define MAX_TARGETS 100
#define MAX_DEPENDENCIES 10

bool show_graph = false; 

typedef struct {
    char target[256];
    char dependencies[MAX_DEPENDENCIES][256];
    int dependency_count;
    int indegree; 
} Node;

Node graph[MAX_TARGETS];
int graph_size = 0;

// 添加目标和依赖
void add_dependency(const char *target, const char *dependency) {
    // 过滤掉与 minimake 相关的目标和依赖
    if (strcmp(target, "minimake") == 0 || strcmp(dependency, "minimake") == 0) {
        return;
    }

    int target_index = -1;

    // 查找目标是否已存在
    for (int i = 0; i < graph_size; i++) {
        if (strcmp(graph[i].target, target) == 0) {
            target_index = i;
            break;
        }
    }

    // 如果目标不存在，添加新节点
    if (target_index == -1) {
        target_index = graph_size++;
        strcpy(graph[target_index].target, target);
        graph[target_index].dependency_count = 0;
        graph[target_index].indegree = 0;
    }

    // 检查是否已经存在该依赖
    for (int i = 0; i < graph[target_index].dependency_count; i++) {
        if (strcmp(graph[target_index].dependencies[i], dependency) == 0) {
            // 如果依赖已经存在，跳过添加
            return;
        }
    }

    // 添加依赖
    if (graph[target_index].dependency_count < MAX_DEPENDENCIES) {
        strcpy(graph[target_index].dependencies[graph[target_index].dependency_count++], dependency);
    } else {
        printf("警告: 目标 '%s' 的依赖数量超过限制 (%d)\n", target, MAX_DEPENDENCIES);
        return;
    }

    // 查找依赖是否已存在于图中
    int dependency_index = -1;
    for (int i = 0; i < graph_size; i++) {
        if (strcmp(graph[i].target, dependency) == 0) {
            dependency_index = i;
            break;
        }
    }

    if (dependency_index == -1) {
        // 如果依赖不存在，添加新节点
        dependency_index = graph_size++;
        strcpy(graph[dependency_index].target, dependency);
        graph[dependency_index].dependency_count = 0;
        graph[dependency_index].indegree = 0;
    }

    // 更新依赖的入度
    graph[dependency_index].indegree++;

    if (show_graph) {
        printf("添加依赖: %s -> %s\n", target, dependency);
    }
}
// 打印依赖关系图
void print_graph() {
    printf("依赖关系图:\n");
    for (int i = 0; i < graph_size; i++) {
        // 跳过与 minimake 相关的目标
        if (strcmp(graph[i].target, "minimake") == 0) {
            continue;
        }

        printf("目标: %s, 入度: %d, 依赖: ", graph[i].target, graph[i].indegree);
        for (int j = 0; j < graph[i].dependency_count; j++) {
            if (strcmp(graph[i].dependencies[j], "minimake") == 0) {
                continue;
            }
            printf("%s ", graph[i].dependencies[j]);
        }
        printf("\n");
    }
    printf("图中共有 %d 个目标\n", graph_size);
}