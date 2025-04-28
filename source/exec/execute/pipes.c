/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipes.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: eklymova <eklymova@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/02/20 13:34:38 by eklymova      #+#    #+#                 */
/*   Updated: 2025/04/28 14:36:45 by Mika Schipp   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */


#include "../../../include/pipex_bonus.h"
#include "../../../include/structbuild.h"
#include "../../../include/memory.h"
#include "../../../include/builtins.h"


//TODO rederection for singl our own cmd malloc_pipes(malloc) leaking)

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

void open_files(t_command *commands)
 {
	t_redirect	*redirects;
	redirects = commands->redirects;

	while (redirects)
	{
		/// if (heredoc detected)
	// 	// here_doc(commands.args[1], commands.argc);
		if (redirects->type == RE_INPUT)
		{	
			redirects->in_fd = open(redirects->file, O_RDONLY);
			if (redirects->in_fd == -1)
			{
				if (errno == ENOENT)
					return (ft_putstr_fd(" No such file or directory\n", 2), exit(1));
				else if (errno == EACCES)
					return (ft_putstr_fd(" Permission denied\n", 2), exit(1));
			}
		}
		if (redirects->type == RE_OUTPUT_TRUNC) 
			redirects->out_fd = open(redirects->file, //if > then we need to do trunc
				O_WRONLY | O_CREAT | O_TRUNC, 0777);
		else if (redirects->type == RE_OUTPUT_APPEND)
		{	
			redirects->out_fd = open(redirects->file, //if >> then we need to do append
				O_WRONLY | O_CREAT | O_APPEND, 0777);
			if (redirects->out_fd == -1)
			{
				if (errno == ENOENT)
					return (ft_putstr_fd(" No such file or directory\n", 2), exit(1));
				else if (errno == EACCES)
					return (ft_putstr_fd(" Permission denied\n", 2), exit(1));
			}
		}
		redirects = redirects->next;
	}
 }


void	redirection(int i, int **pipes, t_command *commands, size_t cmdcount)
{
	t_redirect *redirects = commands->redirects;
	bool has_in = false;
	bool has_out = false;

	while (redirects)
	{
		if (redirects->type == RE_INPUT)
		{
			 if (dup2(redirects->in_fd, STDIN_FILENO) == -1)
				 error(1);
			close(redirects->in_fd);
			has_in = true;
		}
		if (redirects->type == RE_OUTPUT_TRUNC ||
			redirects->type == RE_OUTPUT_APPEND)
		{
	 		if (dup2(redirects->out_fd, STDOUT_FILENO) == -1)
	 			error(1);
			close(redirects->out_fd);
			has_out = true;
		}
		redirects = redirects->next;
	}
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

void	child_process(t_shell shell, int i, int **pipes, char *envp[], t_command *cmds, size_t cmdcount)
{
	if (cmds->has_redirects)
		open_files(cmds);	
	redirection(i, pipes, cmds, cmdcount);
	close_fd(cmds, pipes, cmdcount);
	execute(shell, cmds, envp);
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

void fork_plz(t_shell shell, t_command *commands, int **pipes, char **envp, size_t cmdcount)
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
			child_process(shell, i, pipes, envp, commands, cmdcount);
			exit(1);
		}
		i++;
		commands = commands->next;
	}
}

int	pipes(t_shell shell, t_command *cmds, char *envp[], size_t cmdcount, int *status)
{
	int			**pipes;
	int			i;
	t_command 	*commands;
	int			temp_status;

	pipes = malloc_pipes(cmds, cmdcount);
	if (!pipes)
		return (1);
	create_pipes(cmdcount, pipes);
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
	free_array((void **)pipes, NULL); 
	return (0);
}

int	execute_signal_cmd(t_shell shell, t_command *cmds, char *envp[], int *status)
{
	pid_t	pid;


	if (is_builtins(cmds, envp))
	{
		execve_builtin(shell, cmds, envp);
		exit(EXIT_SUCCESS);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork failed");
		return (1);
	}
	if (pid == 0)
	{
		if (cmds->has_redirects)
			open_files(cmds);	
		redirection(0, 0, cmds, 1);
		close_fd(cmds, 0, 1);
		execute(shell, cmds, envp);
		exit(0);
	}
	waitpid(pid, status, 0);
	return (0);
}

int execute_cmds(t_shell shell, t_command *cmds, char *envp[], size_t cmdcount)
{
	int status;

	status = 0;
	if (cmdcount == 1)
		execute_signal_cmd(shell, cmds, envp, &status);
	else
		pipes(shell, cmds, envp, cmdcount, &status);
	if (WIFEXITED(status))
		return WEXITSTATUS(status);
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}
