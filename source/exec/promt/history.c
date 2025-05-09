/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mika <mika@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 18:10:39 by eklymova          #+#    #+#             */
/*   Updated: 2025/05/09 13:38:21 by mika             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../../lib/libft/libft.h"
#include "../../../include/builtins.h"

/**
 * TODO: Get this thing into home dir instead of current
 * TODO: Add a limit to the history file (like 1000 lines or whatever)
 * TODO: Somewhere else than in this function: READ this function and add_history for each line :)
 */

int	get_history(void)
{
	char	*line;
	int		fd;

	fd = open(".minishell_history", O_RDONLY | O_CREAT, 0644);
	if (fd == -1)
		return (1);
	line = get_next_line(fd);
	while (line)
	{
		line[ft_strlen(line) - 1] = '\0';
		add_history(line);
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	return (0);
}
int	history(char *line)
{
	int	res;
	int	fd;

	if (!line)
		return (0);
	fd = open(".minishell_history", O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
		return (1);
	res = write(fd, line, ft_strlen(line));
	if (res == -1)
		return (1);
	res = write(fd, "\n", 1);
	close(fd);
	if (res == -1)
		return (1);
	return (0);
}