/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mika <mika@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 15:08:06 by mschippe          #+#    #+#             */
/*   Updated: 2025/05/09 14:15:58 by mika             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"
#include "../../../include/builtins.h"
#include "../../../include/variable.h"
#include "../../../include/structbuild.h"
#include "../../../include/memory.h"

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

char	*expand_heredoc_line(t_shell *shell, char *line, bool expand)
{
	char		*exp;
	t_env_var	**vars;
	
	if (!line || !expand)
		return (line);
	vars = get_vars_from_cmd(line, shell, false);
	if (!vars)
		return (free(line), NULL);
	exp = get_expanded_cmd(line, vars, false);
	if (!exp)
		return (free(line), free_array((void **)vars, &clear_env_var), NULL);
	free(line);
	free_array((void **)vars, &clear_env_var);
	return (exp);
}

char	*heredoc_prompt(void)
{
	char	*line;
	char	*res;
	
	if (isatty(fileno(stdin)))
		res = readline("> ");
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

char	*get_heredoc(t_shell *shell, char *delim, bool expand)
{
	char	*total;
	char	*line;
	char	*tmp;

	total = ft_strdup("");
	line = NULL;
	while ((!line || !strequals(delim, line)) && total)
	{
		free(line);
		line = expand_heredoc_line(shell, heredoc_prompt(), expand);
		if (!line)
			break ;
		else if (!strequals(delim, line))
		{
			tmp = total;
			total = ft_strjoin_nl(total, line);
			free(tmp);
		}
	}
	free(line);
	return (total);
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

char	*make_heredoc_file(t_shell *shell, char *delim, char *value)
{
	char	*path;
	int		fd;
	int		write_result;

	path = generate_filename(shell, delim);
	if (!path)
		return (NULL);
	fd = open(path, O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return (free(path), NULL);
	write_result = write(fd, value, ft_strlen(value));
	if (write_result == -1)
		return (close(fd), free(path), NULL);
	close(fd);
	return (path);
}

char	*heredoc(t_shell *shell, char *delim, bool expand)
{
	char		*res;
	char		*path;
	t_env_var	**vars;

	res = get_heredoc(shell, delim, expand);
	if (!res)
		return (NULL);
	path = make_heredoc_file(shell, delim, res);
	return (free(res), path);
}

bool	exec_heredocs(t_shell *shell, t_command *cmd)
{
	t_redirect	*r;
	while (cmd)
	{
		if (!cmd->has_redirects)
		{
			cmd = cmd->next;
			continue ;
		}
		r = cmd->redirects;
		while (r)
		{
			if (r->type != RE_HEREDOC)
			{
				r = r->next;
				 continue ;
			}
			r->file = heredoc(shell, r->heredoc_delim, r->expand_in_heredoc);
			if (!r->file)
				return (false);
			r = r->next;
		}
		cmd = cmd->next;
	}
	return (true);
}
