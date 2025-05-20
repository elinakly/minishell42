/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschippe <mschippe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 15:35:57 by eklymova          #+#    #+#             */
/*   Updated: 2025/05/20 18:02:42 by mschippe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	strequals(char *one, char *two)
{
	size_t	index;

	index = 0;
	while (one[index] && two[index] && one[index] == two[index])
		index++;
	return (!one[index] && !two[index]);
}

/**
 * @brief Check if the command is a built-in command
 * 
 * @param command the command to check
 * @return int 1 if the command is a built-in command, 0 otherwise
 **/

bool	is_builtins(t_command *cmds)
{
	if (cmds && cmds->has_command)
	{
		if (strequals(cmds->name, "echo"))
			return (true);
		if (strequals(cmds->name, "pwd"))
			return (true);
		if (strequals(cmds->name, "env"))
			return (true);
		if (strequals(cmds->name, "exit"))
			return (true);
		if (strequals(cmds->name, "cd"))
			return (true);
		if (strequals(cmds->name, "export"))
			return (true);
		if (strequals(cmds->name, "unset"))
			return (true);
		if (strequals(cmds->name, "clear"))
			return (true);
		cmds = cmds->next;
	}
	return (false);
}

int	execve_builtin(t_shell *shell, t_command *cmds,
		char **envp)
{
	if (cmds && cmds->has_command)
	{
		if (strequals(cmds->name, "echo"))
			return (echo(cmds->argv));
		if (strequals(cmds->name, "pwd"))
			return (pwd());
		if (strequals(cmds->name, "env"))
			return (env(envp));
		if (strequals(cmds->name, "exit"))
			return (ft_exit(shell, cmds->argv));
		if (strequals(cmds->name, "cd"))
			return (cd(shell, cmds));
		if (strequals(cmds->name, "export"))
			return (export(shell, cmds, envp));
		if (strequals(cmds->name, "unset"))
			return (unset(shell, cmds, envp));
		if (strequals(cmds->name, "clear"))
			return (ft_clear());
		cmds = cmds->next;
	}
	return (0);
}
