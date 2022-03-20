#pragma once

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>

#define AV **av
#define IS_FG(x) (strcmp(x, ";") == 0)
#define IS_PIPE(x) (strcmp(x, "|") == 0)
#define IS_CD(x) (strcmp(x, "cd") == 0)
#define FATAL write(2, "error: fatal\n", sizeof("error: fatal"));  exit(1);
#define NLN(fd) write(fd, "\n", 1);
#define EXEC_ERR(x) write(2, "error: cannot execute ", sizeof("error: cannot execute")); putstr(2, x); NLN(2);
#define IN 0
#define OUT 1

typedef enum {
    NA,
    PIPE,
    FG,
    EXEC
}   t_type;

typedef struct  s_tree {
    t_type type;
    char **cmd;
    struct s_tree *left;
    struct s_tree *right;
}               t_tree;

t_tree *parse(char ***av);
void exec(t_tree *tree, char **env);

void putstr(int fd, char *str);
t_tree *new_tree(t_type type, char **cmd, t_tree *left, t_tree *right);