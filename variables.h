#ifndef VARIABLES_H
#define VARIABLES_H


//解析变量
void parse_makefile_variables(const char *makefile_path);

// 添加或更新变量
void set_variable(const char *name, const char *value);

// 获取变量值
const char *get_variable(const char *name);

// 解析字符串中的变量
char *expand_variables(const char *input);

// 清理变量存储
void clear_variables();

// 执行命令并替换变量
void execute_command_with_variables(const char *command);
//打印存储变量
void print_variables();
#endif // VARIABLES_H