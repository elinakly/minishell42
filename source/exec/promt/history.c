/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschippe <mschippe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 18:10:39 by eklymova          #+#    #+#             */
/*   Updated: 2025/05/24 16:50:26 by mschippe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * TODO: Get this thing into home dir instead of current
 * TODO: Add a limit to the history file (like 1000 lines or whatever)
 * TODO: Somewhere else than in this function: READ this
 * 		function and add_history for each line :)
 */
int	get_history(t_shell *shell)
{
	char	*line;
	char	*dir;
	int		fd;

	if (!shell->loop_active)
		return (1);
	dir = expand_dir(shell, "~/.minishell_history");
	if (!dir)
		return (1);
	fd = open(dir, O_RDONLY | O_CREAT, 0644);
	if (fd == -1)
		return (free(dir), 1);
	line = get_next_line(fd);
	while (line)
	{
		line[ft_strlen(line) - 1] = '\0';
		add_history(line);
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	return (free(dir), 0);
}

int	history(t_shell *shell, char *line)
{
	int		res;
	int		fd;
	char	*dir;

	if (!line)
		return (0);
	dir = expand_dir(shell, "~/.minishell_history");
	if (!dir)
		return (1);
	fd = open(dir, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
		return (free(dir), 1);
	res = write(fd, line, ft_strlen(line));
	if (res == -1)
		return (free(dir), 1);
	res = write(fd, "\n", 1);
	close(fd);
	if (res == -1)
		return (free(dir), 1);
	return (free(dir), 0);
}
