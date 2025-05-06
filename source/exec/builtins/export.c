#include "builtins.h"
#include "venv.h"
#include "minishell.h"

int valid_input(char *name)
{
    if (!isalpha(name[0]) && name[0] != '_')
        return (0);
    while (*name && *name != '=')
    {   
        if (!isalnum(*name) && *name != '_')
            return (0);
        name++;
    }
    return (1);
}


int export(t_shell shell, t_command *cmds, char **envp)
{
    t_venv *new_var;
    int status;
    int i;
    char name[1024];
    char *value = NULL;
    int j;
    
    i = 1;
    status = 0;
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
        {
            ft_putstr_fd("minishell: export: not a valid identifier\n", 2);
            status = 1;
        }
        i++;
    }
    i = 1;
    while (i < cmds->argc)
    { 
        if (valid_input(cmds->argv[i]))
        {
            j = 0;
            while (cmds->argv[i][j] && cmds->argv[i][j] != '=')
            {
                name[j] = cmds->argv[i][j];
                j++;
            }
            name[j] = '\0';
            value = ft_strchr(cmds->argv[i], '=');
            if (value)
                value++;
        
            if (!simple_add_var(shell.venv, name, value))
                return (1);
        }
        i++;
    }
    return (status);
}
