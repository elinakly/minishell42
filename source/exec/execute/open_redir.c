/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschippe <mschippe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 19:58:46 by eklymova          #+#    #+#             */
/*   Updated: 2025/05/16 20:49:32 by mschippe         ###   ########.fr       */
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
				return (ft_putstr_fd(" No such file or directory\n", 2), 127);
			else if (errno == EACCES)
				return (ft_putstr_fd(" Permission denied\n", 2), 126);
		}
	}
	return (1);
}

int	open_out_files(t_redirect	*redirects)
{
	if (redirects->type == RE_OUTPUT_TRUNC)
		redirects->out_fd = open(redirects->file,
				O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (redirects->type == RE_OUTPUT_APPEND)
	{
		redirects->out_fd = open(redirects->file,
				O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (redirects->out_fd == -1)
		{
			if (errno == ENOENT)
				return (ft_putstr_fd(" No such file or directory\n", 2), 127);
			else if (errno == EACCES)
				return (ft_putstr_fd(" Permission denied\n", 2), 126);
		}
	}
	return (1);
}

int	open_files(t_shell *shell, t_command *commands)
{
	t_redirect	*redirects;
	int			status;

	redirects = commands->redirects;
	status = 0;
	while (redirects)
	{
		status = open_in_files(redirects);
		if (status != 1)
			return (not_so_fake_exit(shell, status));
		status = open_out_files(redirects);
		if (status != 1)
			return (not_so_fake_exit(shell, status));
		redirects = redirects->next;
	}
	return (status);
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

void	redirection(t_shell *shell, int i, int **pipes, t_command *commands)
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
	if (i < shell->cmds_count - 1 && has_out == false)
	{
		if (dup2(pipes[i][1], STDOUT_FILENO) == -1)
		{
			fprintf(stderr, "dup2 failed2\n");
			error(1);
		}
		close(pipes[i][1]);
	}
}
