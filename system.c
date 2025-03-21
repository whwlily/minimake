#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int my_system(const char *command) {
    if (command == NULL) {
        fprintf(stderr, "Command is NULL.\n");
        return -1;
    }

    pid_t pid = fork(); // 创建子进程
    if (pid < 0) {
        perror("fork failed");
        return -1;
    }

    if (pid == 0) {
        // 子进程中执行命令
        char *args[] = {"/bin/sh", "-c", (char *)command, NULL};
        execvp(args[0], args);
        // 如果 execvp 返回，说明出错
        perror("execvp failed");
        exit(EXIT_FAILURE);
    } else {
        // 父进程中等待子进程完成
        int status;
        if (waitpid(pid, &status, 0) == -1) {
            perror("waitpid failed");
            return -1;
        }

        // 检查子进程退出状态
        if (WIFEXITED(status)) {
            int exit_status = WEXITSTATUS(status);
            if (exit_status != 0) {
                fprintf(stderr, "Command failed with exit status %d\n", exit_status);
                return exit_status;
            }
            return 0; // 成功
        } else if (WIFSIGNALED(status)) {
            fprintf(stderr, "Command terminated by signal %d\n", WTERMSIG(status));
            return -1;
        } else {
            fprintf(stderr, "Unknown child process status\n");
            return -1;
        }
    }
}