// allowed
// malloc, free, write, close, fork, waitpid, signal, kill, exit, chdir, execve, dup, dup2, pipe, strcmp, strncmp
#include "mysh.h"

// void disp_tree(t_tree *tree, int lev);

int main(int ac, char **av, char **env)
{
    // skip av[0]
    av++;

    // parse
    t_tree *tree = parse(&av);

    // disp_tree(tree, 0);
    
    // exec
    exec(tree, env);

    exit(0);
    (void)ac;
    (void)av;
    (void)env;
}

void putstr(int fd, char *str)
{
    while (*str)
        write(fd, str++, 1);
}

void disp_tree(t_tree *tree, int lev) {
    if (!tree)
        return ;
    for (int i = 0; i < lev; i++)
        printf("\t\t");
    if (tree->type == PIPE) {
        printf("PIPE\n");
    } else if (tree->type == FG) {
        printf("FG\n");
    } else if (tree->type == EXEC) {
        printf("EXEC: ");
        for (int i = 0; tree->cmd[i]; i++) {
            printf(" %s  ", tree->cmd[i]);
        }
        printf("\n");
    } else {
        printf("UNK\n");
    }
    disp_tree(tree->left, lev + 1);
    disp_tree(tree->right, lev + 1);
}