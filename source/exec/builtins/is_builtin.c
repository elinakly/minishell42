/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eklymova <eklymova@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 15:35:57 by eklymova          #+#    #+#             */
/*   Updated: 2025/03/07 17:42:40 by eklymova         ###   ########.fr       */
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

int		is_builtin(char *command)
{
	if (ft_strncmp(command, "echo", 5) == 0) //TODO: Hi Elina I changed this to strncmp cus libft doesnt have strcmp
		return (echo(command));
	// if (ft_strncmp(command, "cd") == 0)
	// 	return (1);
	// if (ft_strncmp(command, "pwd") == 0)
	// 	return (1);
	// if (ft_strncmp(command, "env") == 0)
	// 	return (1);
	// if (ft_strncmp(command, "export") == 0)
	// 	return (1);
	// if (ft_strncmp(command, "unset") == 0)
	// 	return (1);
	return (0);
}