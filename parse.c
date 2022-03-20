#include "mysh.h"

int accept_fg(char ***av)
{
    int flag = 0;
    while (**av && IS_FG(**av)) {
        flag = 1;
        **av = NULL;
        (*av)++;
    }
    return flag;
}

t_tree *parse_list(char ***av)
{
    t_tree *ret;

    if (!**av) return NULL;
    ret = new_tree(EXEC, *av, 0, 0);
    while (**av && !IS_FG(**av) && !IS_PIPE(**av)) {
        (*av)++;
    }
    return (ret);
}

t_tree *parse_pline(char ***av)
{
    t_tree *ret;

    if (!**av)
        return NULL;
    ret = parse_list(av);
    if (!ret)
        return NULL;
    while (**av && IS_PIPE(**av))
    {
        **av = NULL;
        (*av)++;
        ret = new_tree(PIPE, 0, ret, NULL);
        ret->right = parse_list(av);
        if (!ret->right)
            return NULL;
    }
    return ret;
}

t_tree *parse(char ***av)
{
    t_tree *ret;

    if (!**av)
        return NULL;
    ret = parse_pline(av);
    if (!ret)
        return NULL;
    if (**av && accept_fg(av)) {
        ret = new_tree(FG, 0, ret, parse(av));
    }
    return ret;
}

t_tree *new_tree(t_type type, char **cmd, t_tree *left, t_tree *right)
{
    t_tree *ret = malloc(sizeof(t_tree));
    if (!ret)
        {FATAL}
    ret->type = type;
    ret->cmd = cmd;
    ret->right = right;
    ret->left = left;
    return ret;
}