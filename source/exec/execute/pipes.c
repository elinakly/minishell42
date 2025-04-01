/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eklymova <eklymova@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 13:34:38 by eklymova          #+#    #+#             */
/*   Updated: 2025/03/28 20:25:26 by eklymova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../../include/pipex_bonus.h"
#include "../../../include/structbuild.h"
#include "../../../include/memory.h"
#include "../../../include/builtins.h"


//TODO rederection input and output files , malloc_pipes(malloc) leaking)

void	create_pipes(int num_cmds, int **pipes)
{
	int	i;

	i = 0;
	while (i < num_cmds - 1)
	{
		if (pipe(pipes[i]) == -1)
		{
			perror("pipe failed");
			exit(EXIT_FAILURE);
		}
		i++;
	}
}

 void open_files(t_command *commands, int i, size_t cmdcount)
 {
 	//int input_fd;
 	//int output_fd;

	while (commands->redirects)
	{
		/// if (heredoc detected)
	// 	// here_doc(commands.args[1], commands.argc);
		if (commands->redirects->type == RE_INPUT)
		{	
			commands->redirects->in_fd = open(commands->redirects->file, O_RDONLY);
			if (commands->redirects->in_fd == -1)
				return ;
		}
		if (commands->redirects->type == RE_OUTPUT_TRUNC) 
			commands->redirects->out_fd = open(commands->redirects->file, //if > then we need to do trunc
				O_WRONLY | O_CREAT | O_TRUNC, 0777);
		else if (commands->redirects->type == RE_OUTPUT_APPEND)
		{	
			commands->redirects->out_fd = open(commands->redirects->file, //if >> then we need to do append
				O_WRONLY | O_CREAT | O_APPEND, 0777);
			if (commands->redirects->out_fd == -1)
				return ;
		}
		commands->redirects = commands->redirects->next;
	}
 }


void	redirection_for_pipes(int i, int **pipes, t_command *commands, size_t cmdcount)
{
	if (commands->has_redirects && commands->redirects->type == RE_INPUT)
	{
	 	if (dup2(commands->redirects->in_fd, STDIN_FILENO) == -1)
		 	error(1);
		close(commands->redirects->in_fd);
	}
	else
		if (i > 0)
		{
			if (dup2(pipes[i - 1][0], STDIN_FILENO) == -1)
			{
				fprintf(stderr, "dup2 failed1\n");
				error(1);
			}
			close(pipes[i - 1][0]);
		}
	if (commands->has_redirects && (commands->redirects->type == RE_OUTPUT_TRUNC ||
			commands->redirects->type == RE_OUTPUT_APPEND))
	{
	 	if (dup2(commands->redirects->out_fd, STDOUT_FILENO) == -1)
	 		error(1);
		close(commands->redirects->out_fd);
	}
	else
		if (i < cmdcount - 1)
		{
			if (dup2(pipes[i][1], STDOUT_FILENO) == -1)
			{
				fprintf(stderr, "dup2 failed2\n");
				error(1);
			}
			close(pipes[i][1]);
		}
}


void	child_process(int i, int **pipes, char *envp[], t_command *cmds, size_t cmdcount)
{
	if (cmds->has_redirects)
		open_files(cmds, i, cmdcount);	
	redirection_for_pipes(i, pipes, cmds, cmdcount);
	close_fd(cmds, pipes, cmdcount);
	execute(cmds, envp);
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

void	fork_plz(t_command *commands, int **pipes, char **envp, size_t cmdcount)
{
	int		i;
	pid_t	pid;

	i = 0;
	while (i < cmdcount)
	{
		pid = fork();
		if (pid == -1)
		{
			perror("fork failed");
			return ;
		}
		if (pid == 0)
			child_process(i, pipes, envp, commands, cmdcount);
		i++;
		commands = commands->next;
	}
}

int	pipes(t_command *cmds, char *envp[], size_t cmdcount)
{
	int			**pipes;
	int			i;

	pipes = malloc_pipes(cmds, cmdcount);
	if (!pipes)
		return (1);
	create_pipes(cmdcount, pipes);
	fork_plz(cmds, pipes, envp, cmdcount);
	close_fd(cmds, pipes, cmdcount);
	i = 0;
	while (i < cmdcount)
	{
		wait(NULL);
		i++;
	}
	free_array((void **)pipes, NULL);
	return (0);
}

int	execute_signal_cmd(t_command *cmds, char *envp[])
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
	{
		perror("fork failed");
		return (1);
	}
	if (pid == 0)
	{
		execute(cmds, envp);
		exit(0);
	}
	waitpid(pid, &status, 0);
	return (0);
}

int execute_cmds(t_command *cmds, char *envp[], size_t cmdcount)
{
	if (cmdcount == 1)
	{
		if (is_builtins(cmds, envp))
			return (execve_builtin(cmds, envp), 0);
	}
	if (cmdcount == 1)
		execute_signal_cmd(cmds, envp);
	else
		pipes(cmds, envp, cmdcount);
	return (0);
}
