/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_builtIn.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eklymova <eklymova@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 15:35:57 by eklymova          #+#    #+#             */
/*   Updated: 2025/03/04 15:36:31 by eklymova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


/**
 * @brief Check if the command is a built-in command
 * 
 * @param command the command to check
 * @return int 1 if the command is a built-in command, 0 otherwise
 **/

int		is_builtin(char *command)
{
	if (ft_strcmp(command, "echo") == 0)
		return (1);
	// if (ft_strcmp(command, "cd") == 0)
	// 	return (1);
	// if (ft_strcmp(command, "pwd") == 0)
	// 	return (1);
	// if (ft_strcmp(command, "env") == 0)
	// 	return (1);
	// if (ft_strcmp(command, "export") == 0)
	// 	return (1);
	// if (ft_strcmp(command, "unset") == 0)
	// 	return (1);
	return (0);
}