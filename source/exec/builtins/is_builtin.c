/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   is_builtin.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: eklymova <eklymova@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/03/04 15:35:57 by eklymova      #+#    #+#                 */
/*   Updated: 2025/04/28 15:53:53 by Mika Schipp   ########   odam.nl         */
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

bool	is_builtins(t_command *cmds, char **envp)
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
		cmds = cmds->next;
	}
	return (false);
}

void	execve_builtin(t_shell shell, t_command *cmds, char **envp)
{
	if (cmds && cmds->has_command)
	{
		if (strequals(cmds->name, "echo"))
			echo(cmds->argv);
		if (strequals(cmds->name, "pwd"))
			pwd();
		if (strequals(cmds->name, "env"))
			env(envp);
		if (strequals(cmds->name, "exit"))
			ft_exit(cmds->argv);
		if (strequals(cmds->name, "cd"))
			cd(shell, cmds);
		cmds = cmds->next;
	}
}
