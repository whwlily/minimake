
int check_makefile();
int parse_makefile(const char *filename);
void check_rules();
void run_command(const char *target);
void debug_print_variables();
int check_dependency_validity(const char *dep);