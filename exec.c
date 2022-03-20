#include "mysh.h"

int fork1();
int pipe1(int fd[2]);

void cd(char **cmd) {
    int ac = 0;
    while (cmd[ac])
        ac++;
    if (ac != 2) {
        write(2, "error: cd: bad arguments\n", sizeof("error: cd: bad arguments"));
        return ;
    }
    int ret = chdir(cmd[1]);
    if (ret == -1) {
        write(2, "error: cd: cannot change directory to ", sizeof("error: cd: cannot change directory to"));
        putstr(2, cmd[1]);
        NLN(2);
        return ;
    }
    return ;
}

void exec(t_tree *tree, char **env)
{
    if (!tree)
        return ;
    
    if (tree->type == EXEC) {
        
        if (IS_CD(tree->cmd[0])) {
            return cd(tree->cmd);
        }

        int pid = fork1();
        if (pid == 0) {
           execve(tree->cmd[0], tree->cmd, env);
           EXEC_ERR(tree->cmd[0]);
           exit(1);
        }
        waitpid(pid, NULL ,0);
        return ;
    }

    if (tree->type == FG) {
        exec(tree->left, env);
        exec(tree->right, env);
        return ;
    }

    if (tree->type == PIPE) {
        int pfd[2];

        // create pipe
        pipe1(pfd);

        // exec left
        if (fork1() == 0) {
            // dup output
            close(OUT);
            dup2(pfd[OUT], OUT);
            close(pfd[OUT]);
            close(pfd[IN]);
            exec(tree->left, env);
            exit(0);
        }

        // exec right
        if (fork1() == 0) {
            // dup input
            close(IN);
            dup2(pfd[IN], IN);
            close(pfd[IN]);
            close(pfd[OUT]);
            exec(tree->right, env);
            exit(0);
        }

        close(pfd[IN]);
        close(pfd[OUT]);
        waitpid(-1, 0, 0);
        waitpid(-1, 0, 0);

        return ;
    }
    return ;
}

int fork1()
{
    int pid = fork();
    if (pid == -1)
        {FATAL}
    return pid;
}

int pipe1(int fd[2])
{
    int ret = pipe(fd);
    if (ret == -1)
        {FATAL}
    return ret;
}