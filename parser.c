#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>

#define MAX_ENTRIES 100
#define MAX_DEPENDENCIES 10

typedef struct {
    char target[33];
    char dependencies[MAX_DEPENDENCIES][33];
    int dependency_count;
    char command[256];
} MakefileEntry;

MakefileEntry entries[MAX_ENTRIES];
int entry_count = 0;

// 检查文件是否存在
bool file_exists(const char *filename) {
    struct stat buffer;
    return (stat(filename, &buffer) == 0);
}

// 检查目标是否已定义
bool target_exists(const char *target) {
    for (int i = 0; i < entry_count; i++) {
        if (strcmp(entries[i].target, target) == 0) {
            return true;
        }
    }
    return false;
}

// 解析 Makefile
void parse_makefile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening Makefile");
        exit(1);
    }

    char line[512];
    int line_number = 0;
    while (fgets(line, sizeof(line), file)) {
        line_number++;
        // 跳过空行或注释
        if (line[0] == '\n' || line[0] == '#') continue;

        // 解析目标和依赖
        if (strchr(line, ':')) {
            char *target = strtok(line, ":");
            char *deps = strtok(NULL, "\n");

            // 去掉多余空格
            target = strtok(target, " ");
            if (target_exists(target)) {
                fprintf(stderr, "Line %d: Duplicate target definition '%s'\n", line_number, target);
                exit(1);
            }

            MakefileEntry entry;
            strncpy(entry.target, target, 32);
            entry.dependency_count = 0;

            // 解析依赖
            char *dep = strtok(deps, " ");
            while (dep) {
                if (!file_exists(dep) && !target_exists(dep)) {
                    fprintf(stderr, "Line %d: Invalid dependency '%s'\n", line_number, dep);
                    exit(1);
                }
                strncpy(entry.dependencies[entry.dependency_count++], dep, 32);
                dep = strtok(NULL, " ");
            }

            entries[entry_count++] = entry;
        } else {
            // 解析命令
            if (entry_count == 0) {
                fprintf(stderr, "Line %d: Command without target\n", line_number);
                exit(1);
            }
            strncpy(entries[entry_count - 1].command, line, 255);
        }
    }

    fclose(file);
}

// 打印解析结果
void print_entries() {
    for (int i = 0; i < entry_count; i++) {
        printf("Target: %s\n", entries[i].target);
        printf("Dependencies: ");
        for (int j = 0; j < entries[i].dependency_count; j++) {
            printf("%s ", entries[i].dependencies[j]);
        }
        printf("\nCommand: %s\n", entries[i].command);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <Makefile>\n", argv[0]);
        return 1;
    }

    parse_makefile(argv[1]);
    print_entries();
    return 0;
}
