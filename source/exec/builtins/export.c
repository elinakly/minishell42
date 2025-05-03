#include "builtins.h"
#include "venv.h"
#include "minishell.h"

int valid_input(char *name)
{
    if (!isalpha(name[0]) && name[0] != '_')
        return (ft_putstr_fd("minishell: export: not a valid identifier\n", 2), 0);
    while (*name && *name != '=')
    {   
        if (!isalnum(*name) && *name != '_')
            return (ft_putstr_fd("minishell: export: not a valid identifier\n", 2), 0);
        name++;
    }
    return (1);
}


int export(t_shell shell, t_command *cmds, char **envp)
{
    char *name;
    char *value = NULL;
    t_venv *new_var;
    int i;
  
    i = 1;
    if (cmds->argc == 1)
    {
        while (*envp)
        {
            printf("declare -x %s\n", *envp);
            envp++;
        }
        return (0);
    }
    while (i < cmds->argc)
    { 
        if (!valid_input(cmds->argv[i]))
            return (1);
        remove_env_var(shell.venv, cmds->argv[i]);
        if (!add_env_var(shell.venv, cmds->argv[i]));
            return (1);
        i++;
    }
    return (0);
}
