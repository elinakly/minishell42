/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eklymova <eklymova@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 15:35:57 by eklymova          #+#    #+#             */
/*   Updated: 2025/03/13 19:57:09 by eklymova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../lib/libft/libft.h"
#include "../../../include/builtins.h"

/**
 * @brief Check if the command is a built-in command
 * 
 * @param command the command to check
 * @return int 1 if the command is a built-in command, 0 otherwise
 **/

int		is_builtin(char *cmd, char **args, char **envp)
{
	if (ft_strncmp(cmd, "echo", 4) == 0)
		return (echo(args));
	// if (ft_strncmp(command, "cd") == 0)
	// 	return (1);
	if (ft_strncmp(cmd, "pwd", 3) == 0)
		return (pwd());
	if (ft_strncmp(cmd, "env", 3) == 0)
		return (env(envp));
	if (ft_strncmp(cmd, "exit", 4) == 0)
		return (ft_exit());
	//TODO: Make export and unset here
	// if (ft_strncmp(command, "export") == 0)
	// 	return (1);
	// if (ft_strncmp(command, "unset") == 0)
	// 	return (1);
	return (0);
}