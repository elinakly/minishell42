/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eklymova <eklymova@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 15:35:57 by eklymova          #+#    #+#             */
/*   Updated: 2025/03/28 19:59:32 by eklymova         ###   ########.fr       */
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
	size_t	len;

	len = ft_strlen(cmds->name);
	while (cmds)
	{
		if (cmds->has_command && ft_strncmp(cmds->name, "echo", len) == 0)
			return (true);
		if (cmds->has_command && ft_strncmp(cmds->name, "pwd", len) == 0)
			return (true);
		if (cmds->has_command && ft_strncmp(cmds->name, "env", len) == 0)
			return (true);
		if (cmds->has_command && ft_strncmp(cmds->name, "exit", len) == 0)
			return (true);
		cmds = cmds->next;
	}
	return (false);
}

void	execve_builtin(t_command *cmds, char **envp)
{
	size_t	len;

	len = ft_strlen(cmds->name);
	while (cmds)
	{
		if (cmds->has_command && ft_strncmp(cmds->name, "echo", len) == 0)
			echo(cmds->argv);
		if (cmds->has_command && ft_strncmp(cmds->name, "pwd", len) == 0)
			pwd();
		if (cmds->has_command && ft_strncmp(cmds->name, "env", len) == 0)
			env(envp);
		if (cmds->has_command && ft_strncmp(cmds->name, "exit", len) == 0)
			ft_exit();
		cmds = cmds->next;
	}
}
