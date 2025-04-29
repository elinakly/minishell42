/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   heredoc.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: mschippe <mschippe@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/04/29 15:08:06 by mschippe      #+#    #+#                 */
/*   Updated: 2025/04/30 00:33:47 by Mika Schipp   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"
#include "../../../include/builtins.h"

char	*ft_strjoin_nl(const char *s1, const char *s2)
{
	char	*res;
	char	*tmp;

	res = ft_strjoin(s1, "\n");
	if (!res)
		return (NULL);
	tmp = res;
	res = ft_strjoin(res, s2);
	if (!res)
		return (NULL);
	free(tmp);
	return (res);
}

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
			total = ft_strjoin_nl(total, line);
			free(tmp);
		}
	}
	free(line);
	return (total);
}
