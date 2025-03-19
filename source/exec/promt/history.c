/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eklymova <eklymova@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 18:10:39 by eklymova          #+#    #+#             */
/*   Updated: 2025/03/19 14:40:44 by eklymova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../../lib/libft/libft.h"
#include "../../../include/builtins.h"

/**
 * TODO: Get this thing into home dir instead of current
 * TODO: Add a limit to the history file (like 1000 lines or whatever)
 * TODO: Somewhere else than in this function: READ this function and add_history for each line :)
 */
int	history(char *line)
{
	int	res;
	int	fd;
	
	fd = open(".minishell_history", O_WRONLY | O_CREAT | O_APPEND, 0777);
	if (fd == -1)
		return (1);
	res = write(fd, line, ft_strlen(line));
	if (res == -1)
		return (1);
	res = write(fd, "\n", 1);
	if (res == -1)
		return (1);
	return (0);
}