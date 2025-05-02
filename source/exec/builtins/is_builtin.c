/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eklymova <eklymova@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 15:35:57 by eklymova          #+#    #+#             */
/*   Updated: 2025/04/29 16:27:06 by eklymova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../lib/libft/libft.h"
#include "../../../include/builtins.h"
#include "../../../include/structbuild.h"

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
		cmds = cmds->next;
	}
	return (false);
}

int	execve_builtin(t_shell shell, t_command *cmds, char **envp, size_t cmdcount)
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
			return(ft_exit(cmds->argv, cmdcount));
		if (strequals(cmds->name, "cd"))
			return (cd(shell, cmds));
		if (strequals(cmds->name, "export"))
			return (export(shell, cmds, envp));
		cmds = cmds->next;
	}
	return (0);	
}
