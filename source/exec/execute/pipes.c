/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eklymova <eklymova@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 13:34:38 by eklymova          #+#    #+#             */
/*   Updated: 2025/05/24 16:48:08 by eklymova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	create_pipes(t_shell *shell, int **pipes)
{
	int	i;

	i = 0;
	while (i < shell->cmds_count - 1)
	{
		if (pipe(pipes[i]) == -1)
		{
			perror("pipe failed");
			not_so_fake_exit(shell, EXIT_FAILURE);
			return ;
		}
		i++;
	}
}

int	child_process(t_shell *shell, int i, int **pipes, t_command *cmds)
{
	int	status;

	if (cmds->has_redirects)
	{
		status = open_files(shell, cmds);
		if (status != 1)
			return (status);
	}
	redirection(shell, i, pipes, cmds);
	close_fd(shell, pipes);
	execute(shell, cmds, shell->venv_arr);
	return (true);
}

int	**malloc_pipes(t_shell *shell)
{
	int	i;
	int	**pipes;

	pipes = malloc(sizeof(int *) * (shell->cmds_count));
	pipes[shell->cmds_count - 1] = NULL;
	if (!pipes)
		return (NULL);
	i = 0;
	while (i < shell->cmds_count - 1)
	{
		pipes[i] = malloc(sizeof(int) * 2);
		if (!pipes[i])
		{
			while (--i >= 0)
				free(pipes[i]);
			free(pipes);
			return (NULL);
		}
		i++;
	}
	return (pipes);
}

bool	fork_plz(t_shell *shell, t_command *commands, int **pipes)
{
	int		i;

	i = 0;
	while (i < shell->cmds_count)
	{
		commands->pid = fork();
		if (commands->pid == -1)
		{
			perror("fork failed");
			return (false);
		}
		if (commands->pid == 0)
		{
			set_child_default_signal();
			if (child_process(shell, i, pipes, commands) != 1)
				return (not_so_fake_exit(shell, 1), false);
			not_so_fake_exit(shell, 1);
			return (true);
		}
		i++;
		commands = commands->next;
	}
	return (true);
}

bool	pipes(t_shell *shell, t_command *cmds, int *status)
{
	set_child_signal();
	shell->pipes = malloc_pipes(shell);
	if (!shell->pipes)
		return (false);
	create_pipes(shell, shell->pipes);
	if (cmds->has_command && !fork_plz(shell, cmds, shell->pipes))
		return (false);
	close_fd(shell, shell->pipes);
	wait_pid(cmds, status);
	if (WIFSIGNALED(*status) && WTERMSIG(*status) == SIGQUIT)
		ft_putstr_fd("Quit (core dumped)\n", 2);
	return (true);
}
