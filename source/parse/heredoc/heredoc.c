/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eklymova <eklymova@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 15:08:06 by mschippe          #+#    #+#             */
/*   Updated: 2025/05/24 16:43:59 by eklymova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

char	*get_heredoc(t_shell *shell, char *delim, bool expand, char *line)
{
	char	*total;
	char	*tmp;
	char	*prompt;

	total = ft_strdup("");
	while ((!line || !strequals(delim, line)) && total)
	{
		free(line);
		prompt = heredoc_prompt();
		if (!prompt)
			return (total);
		if (check_heredoc_cancel(prompt))
			return (free(prompt), free(total), NULL);
		line = expand_heredoc_line(shell, prompt, expand);
		if (!line || strequals(delim, line))
			break ;
		else
		{
			tmp = total;
			total = ft_strjoin_nl(total, line);
			free(tmp);
		}
	}
	return (free(line), total);
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

	set_heredoc_signal();
	res = get_heredoc(shell, delim, expand, NULL);
	if (g_recv_sig == SIGINT)
	{
		shell->status = 130;
		shell->last_parse_res = HEREDOC_CANCEL;
	}
	set_main_signal();
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
