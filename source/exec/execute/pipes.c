/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipes.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: mschippe <mschippe@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/02/20 13:34:38 by eklymova      #+#    #+#                 */
/*   Updated: 2025/03/28 19:38:22 by Mika Schipp   ########   odam.nl         */
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

void	redirection(int i, int **pipes, t_command *commands, size_t cmdcount)
{
	// if (i == 0)
	// 	dup2(commands.input_fd, STDIN_FILENO);
	// else
	if (i > 0)
		dup2(pipes[i - 1][0], STDIN_FILENO);
	// if (i == commands.num_cmds - 1)
	// 	dup2(commands.output_fd, STDOUT_FILENO);
	// else
	if (i < cmdcount - 1)
		dup2(pipes[i][1], STDOUT_FILENO);
}


void	child_process(int i, int **pipes, char *envp[], t_command *cmds, size_t cmdcount)
{
	// commands.input_fd = open(commands.args[1], O_RDONLY);
	// /// if (heredoc detected)
	// // here_doc(commands.args[1], commands.argc);
	// if (commands.input_fd == -1)
	// 	return ;
	// //if ( key isnt apend )
	// 	commands.output_fd = open(commands.args[commands.argc - 1], //if > then we need to do trunc
	// 		O_WRONLY | O_CREAT | O_TRUNC, 0777);
	// //else 
	// //	commands.output_fd = open(commands.args[commands.argc - 1], //if >> then we need to do append
	// //		O_WRONLY | O_CREAT | O_APPEND, 0777);
	// if (commands.output_fd == -1)
	// 	return ;
	redirection(i, pipes, cmds, cmdcount);
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
	if (new_test_exec(cmds, envp))
		return (0);
	if (cmdcount == 1)
		execute_signal_cmd(cmds, envp);
	else
		pipes(cmds, envp, cmdcount);
	return (0);
}
