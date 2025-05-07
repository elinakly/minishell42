#include "builtins.h"
#include "venv.h"
#include "minishell.h"


int		unset(t_shell *shell, t_command *cmds, char **envp)
{
    int     i;

	i = 1;
    if (cmds->argv[1][0] == '-' && cmds->argv[1][1])
        return (ft_putstr_fd("unset: invalid option\n", 2), 2);
    while (i < cmds->argc)
    {
        if (cmds->argv[i][0] != '\0')
            remove_env_var(&shell->venv, cmds->argv[i]);
        i++;
    }
    return (0);
}