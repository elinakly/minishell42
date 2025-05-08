/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mika <mika@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 19:58:46 by eklymova          #+#    #+#             */
/*   Updated: 2025/05/08 19:14:44 by mika             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"
#include "../../../include/structbuild.h"
#include "../../../include/memory.h"
#include "../../../include/builtins.h"

int	open_in_files(t_redirect	*redirects)
{
	if (redirects->type == RE_INPUT || redirects->type == RE_HEREDOC)
	{
		redirects->in_fd = open(redirects->file, O_RDONLY);
		if (redirects->in_fd == -1)
		{
			if (errno == ENOENT)
				return (ft_putstr_fd(" No such file or directory\n", 2), 0);
			else if (errno == EACCES)
				return (ft_putstr_fd(" Permission denied\n", 2), 0);
		}
	}
	return (1);
}

int	open_out_files(t_redirect	*redirects)
{
	if (redirects->type == RE_OUTPUT_TRUNC)
		redirects->out_fd = open(redirects->file,//if > then we need to do trunc
				O_WRONLY | O_CREAT | O_TRUNC, 0777);
	else if (redirects->type == RE_OUTPUT_APPEND)
	{
		redirects->out_fd = open(redirects->file,//if >> then we need to do append
				O_WRONLY | O_CREAT | O_APPEND, 0777);
		if (redirects->out_fd == -1)
		{
			if (errno == ENOENT)
				return (ft_putstr_fd(" No such file or directory\n", 2), 0);
			else if (errno == EACCES)
				return (ft_putstr_fd(" Permission denied\n", 2), 0);
		}
	}
	return (1);
}

int	open_files(t_shell *shell, t_command *commands)
{
	t_redirect	*redirects;

	redirects = commands->redirects;
	while (redirects)
	{
		if (!open_in_files(redirects))
			return (fake_exit(shell, 1));
		if (!open_out_files(redirects))
			return (fake_exit(shell, 1));
		redirects = redirects->next;
	}
	return (0);
}

void	redirects_files(t_command *commands, bool *has_in, bool *has_out)
{
	t_redirect	*redirects;

	redirects = commands->redirects;

	while (redirects)
	{
		if (redirects->type == RE_INPUT || redirects->type == RE_HEREDOC)
		{
			if (dup2(redirects->in_fd, STDIN_FILENO) == -1)
				error(1);
			close(redirects->in_fd);
			*has_in = true;
		}
		if (redirects->type == RE_OUTPUT_TRUNC
			|| redirects->type == RE_OUTPUT_APPEND)
		{
			if (dup2(redirects->out_fd, STDOUT_FILENO) == -1)
				error(1);
			close(redirects->out_fd);
			*has_out = true;
		}
		redirects = redirects->next;
	}
}

void	redirection(int i, int **pipes, t_command *commands, size_t cmdcount)
{
	bool	has_in;
	bool	has_out;

	has_in = false;
	has_out = false;
	redirects_files(commands, &has_in, &has_out);
	if (i > 0 && has_in == false)
	{
		if (dup2(pipes[i - 1][0], STDIN_FILENO) == -1)
		{
			fprintf(stderr, "dup2 failed1\n");
			error(1);
		}
		close(pipes[i - 1][0]);
	}
	if (i < cmdcount - 1 && has_out == false)
	{
		if (dup2(pipes[i][1], STDOUT_FILENO) == -1)
		{
			fprintf(stderr, "dup2 failed2\n");
			error(1);
		}
		close(pipes[i][1]);
	}
}
