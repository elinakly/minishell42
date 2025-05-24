/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eklymova <eklymova@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 16:24:51 by eklymova          #+#    #+#             */
/*   Updated: 2025/05/24 16:43:28 by eklymova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strjoin_nl(const char *s1, const char *s2)
{
	char	*res;
	char	*tmp;

	res = ft_strjoin(s2, "\n");
	if (!res)
		return (NULL);
	tmp = res;
	res = ft_strjoin(s1, res);
	if (!res)
		return (NULL);
	free(tmp);
	return (res);
}

char	*generate_filename(t_shell *shell, char *delim)
{
	char	*addr;
	char	*counter;
	char	*res;
	char	*tmp;
	char	*path;

	addr = ft_itoa((int)((long)&delim % INT_MAX));
	if (!addr)
		return (NULL);
	counter = ft_itoa(shell->heredoc_counter++);
	if (!counter)
		return (free(addr), NULL);
	tmp = ft_strjoin("minishell_heredoc_", addr);
	if (!tmp)
		return (free(addr), free(counter), NULL);
	res = ft_strjoin(tmp, counter);
	if (!res)
		return (free(addr), free(counter), free(tmp), NULL);
	path = ft_strjoin("/tmp/", res);
	return (free(addr), free(counter), free(tmp), free(res), path);
}

char	*heredoc_prompt(void)
{
	char	*line;
	char	*res;

	if (isatty(fileno(stdin)))
	{
		res = readline("> ");
		if (!res)
		{
			printf("minishell: warning: here-document delimited by EOF\n");
			return (NULL);
		}
	}
	else
	{
		line = get_next_line(fileno(stdin));
		if (line)
		{
			res = ft_strtrim(line, "\n");
			free(line);
		}
		else
			return (NULL);
	}
	return (res);
}

bool	check_heredoc_cancel(char *prompt)
{
	if (!prompt)
	{
		printf("minishell: warning: here-document delimited by EOF\n");
		return (false);
	}
	return (g_recv_sig == SIGINT);
}
