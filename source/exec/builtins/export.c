#include "minishell.h"


int export(t_shell shell, t_command *cmds, char **envp)
{
    char *name;
    char *value = NULL;
    name = cmds->argv[1];
    t_venv *new_var;
    
    new_var = malloc(sizeof(t_venv));
    if (cmds->argc == 1)
    {
        while (*envp)
        {
            printf("declare -x %s\n", *envp);
            envp++;
        }
    }
    else
    {
        if (strchr(name, '='))
        {
            value = strchr(name, '=') + 1;
            *strchr(name, '=') = '\0';
        }
        new_var->name = strdup(name);
        new_var->value = value ? strdup(value) : NULL;
        new_var->next = shell.venv;
        shell.venv = new_var;
    }
    return (0);
}