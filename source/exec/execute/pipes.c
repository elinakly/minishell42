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

void	create_pipes(t_shell *shell, int num_cmds, int **pipes)
{
	int	i;

	i = 0;
	while (i < num_cmds - 1)
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

void	child_process(t_shell *shell, int i, int **pipes, char *envp[], t_command *cmds, size_t cmdcount)
{
	if (cmds->has_redirects)
		open_files(shell, cmds);	
	redirection(i, pipes, cmds, cmdcount);
	close_fd(cmds, pipes, cmdcount);
	execute(shell, cmds, envp, cmdcount);
}

int	**malloc_pipes(t_command *commands, size_t cmdcount)
{
	int	i;
	int	**pipes;

	pipes = malloc(sizeof(int *) * (cmdcount));
	pipes[cmdcount - 1] = NULL; // NOTE: I added this so the array can be NULL terminated :)
	if (!pipes)
		return (NULL);
	i = 0;
	while (i < cmdcount - 1)
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

void fork_plz(t_shell *shell, t_command *commands, int **pipes, char **envp, size_t cmdcount)
{
	int		i;
	int		last_pid;

	i = 0;
	while (i < cmdcount)
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
			child_process(shell, i, pipes, envp, commands, cmdcount);
			fake_exit(shell, 1);
			return ;
		}
		i++;
		commands = commands->next;
	}
}

int	pipes(t_shell *shell, t_command *cmds, char *envp[], size_t cmdcount, int *status)
{
	int			**pipes;
	int			i;
	t_command	*commands;
	int			temp_status;

	signal(SIGINT, signal_handler_child);
	pipes = malloc_pipes(cmds, cmdcount);
	if (!pipes)
		return (1);
	create_pipes(shell, cmdcount, pipes);
	fork_plz(shell, cmds, pipes, envp, cmdcount);
	close_fd(cmds, pipes, cmdcount);
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