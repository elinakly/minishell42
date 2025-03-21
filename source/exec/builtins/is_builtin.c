/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eklymova <eklymova@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 15:35:57 by eklymova          #+#    #+#             */
/*   Updated: 2025/03/19 15:42:34 by eklymova         ###   ########.fr       */
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

bool	new_test_exec(t_command *cmds, char **envp)
{
	size_t	len;

	len = ft_strlen(cmds->name);
	while (cmds)
	{
		if (cmds->has_command && ft_strncmp(cmds->name, "echo", len) == 0)
			return (echo(cmds->argv), true);
		if (cmds->has_command && ft_strncmp(cmds->name, "pwd", len) == 0)
			return (pwd(), true);
		if (cmds->has_command && ft_strncmp(cmds->name, "env", len) == 0)
			return (env(envp) , true);
		if (cmds->has_command && ft_strncmp(cmds->name, "exit", len) == 0)
			return (ft_exit(), true);
		cmds = cmds->next;
	}
	return (false);
}
