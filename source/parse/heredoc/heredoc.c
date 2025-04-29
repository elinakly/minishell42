/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschippe <mschippe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 15:08:06 by mschippe          #+#    #+#             */
/*   Updated: 2025/04/29 15:28:31 by mschippe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"
#include "../../../include/builtins.h"

char	*get_heredoc(char *delim, bool expand)
{
	char	*total;
	char	*line;
	char	*tmp;

	total = NULL;
	line = NULL;

	while (!line || !strequals(delim, line))
	{
		free(line);
		line = readline("> ");
		if (!line)
			continue ;
		if (!total)
			total = ft_strdup(line);
		else
		{
			tmp = total;
			total = ft_strjoin(total, line);
			free(tmp);
		}
	}
	free(line);
	return (total);
}
