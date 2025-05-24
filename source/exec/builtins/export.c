/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eklymova <eklymova@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 16:02:42 by eklymova          #+#    #+#             */
/*   Updated: 2025/05/24 16:37:53 by eklymova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sort_env(char **envp)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while (envp[i])
	{
		j = i + 1;
		while (envp[j])
		{
			if (ft_strncmp(envp[i], envp[j], 1024) > 0)
			{
				tmp = envp[i];
				envp[i] = envp[j];
				envp[j] = tmp;
			}
			j++;
		}
		i++;
	}
}

int	valid_input(char *name)
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

int	real_export(char *argv, t_shell *shell)
{
	int		i;
	char	name[1024];
	char	*value;

	i = 0;
	while (argv[i] && argv[i] != '=')
	{
		name[i] = argv[i];
		i++;
	}
	name[i] = '\0';
	value = ft_strchr(argv, '=');
	if (value)
		return (simple_add_var(shell->venv, name, value + 1));
	else
		return (simple_add_var(shell->venv, name, NULL));
	return (0);
}

int	export(t_shell *shell, t_command *cmds, char **envp)
{
	int		status;
	size_t	i;

	i = 0;
	status = 0;
	if (cmds->argc == 1)
	{
		sort_env(envp);
		while (*envp)
			printf("declare -x %s\n", *envp++);
		return (0);
	}
	if (cmds->argv[1][0] == '-' && cmds->argv[1][1])
		return (ft_putstr_fd("minishell: export: invalid option\n", 2), 2);
	while (++i < cmds->argc)
	{
		if (!valid_input(cmds->argv[i]))
		{
			ft_putstr_fd("minishell: export: not a valid identifier\n", 2);
			status = 1;
		}
		else if (!real_export(cmds->argv[i], shell))
			status = 1;
	}
	return (status);
}
