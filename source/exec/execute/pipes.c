/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eklymova <eklymova@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 13:34:38 by eklymova          #+#    #+#             */
/*   Updated: 2025/05/10 18:07:56 by eklymova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../../include/pipex_bonus.h"
#include "../../../include/structbuild.h"
#include "../../../include/memory.h"
#include "../../../include/builtins.h"
#include "execute.h"

//TODO rederection for singl our own cmd malloc_pipes(malloc) leaking)

void	create_pipes(t_shell *shell, int **pipes)
{
	int	i;

	i = 0;
	while (i < shell->cmds_count - 1)
	{
		if (pipe(pipes[i]) == -1)
		{
			perror("pipe failed");
			fake_exit(shell, EXIT_FAILURE);
			return ;
		}
		i++;
	}
}

void	child_process(t_shell *shell, int i, int **pipes, char *envp[], t_command *cmds)
{
	if (cmds->has_redirects)
		open_files(shell, cmds);	
	redirection(shell, i, pipes, cmds);
	close_fd(shell, cmds, pipes);
	execute(shell, cmds, envp);
}

int	**malloc_pipes(t_shell *shell, t_command *commands)
{
	int	i;
	int	**pipes;

	pipes = malloc(sizeof(int *) * (shell->cmds_count));
	pipes[shell->cmds_count - 1] = NULL; // NOTE: I added this so the array can be NULL terminated :)
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

void fork_plz(t_shell *shell, t_command *commands, int **pipes, char **envp)
{
	int		i;
	int		last_pid;

	i = 0;
	while (i < shell->cmds_count)
	{
		commands->pid = fork();
		if (commands->pid == -1)
		{
			perror("fork failed");
			return ;
		}
		if (commands->pid == 0)
		{
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);
			child_process(shell, i, pipes, envp, commands);
			fake_exit(shell, 1);
			return ;
		}
		i++;
		commands = commands->next;
	}
}

int	pipes(t_shell *shell, t_command *cmds, char *envp[], int *status)
{
	int			**pipes;
	int			i;
	t_command	*commands;
	int			temp_status;

	signal(SIGINT, signal_handler_child);
	pipes = malloc_pipes(shell, cmds);
	if (!pipes)
		return (1);
	create_pipes(shell, pipes);
	fork_plz(shell, cmds, pipes, envp);
	close_fd(shell, cmds, pipes);
	commands = cmds;
	temp_status = 0;
	while (commands)
	{
		waitpid(commands->pid, &temp_status, 0);
		if (commands->next == NULL)
			*status = temp_status;
		commands = commands->next;
	}
	if (WIFSIGNALED(*status))
	{
		if (WTERMSIG(*status) == SIGQUIT)
			ft_putstr_fd("Quit (core dumped)\n", 2);
	}
	free_array((void **)pipes, NULL);
	return (0);
}