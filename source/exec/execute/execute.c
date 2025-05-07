/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eklymova <eklymova@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 20:03:06 by eklymova          #+#    #+#             */
/*   Updated: 2025/05/07 20:05:16 by eklymova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"
#include "../../../include/structbuild.h"
#include "../../../include/memory.h"
#include "../../../include/builtins.h"

int	execute(t_shell *shell, t_command *cmd, char **envp, size_t cmdcount)
{
	char	*find_path;

	if (is_builtins(cmd))
		exit(execve_builtin(shell, cmd, envp, cmdcount));
	find_path = find_valid_path(shell, cmd->name, envp);
	if (cmd->name == NULL)
		return (fake_exit(shell, error(3)));
	if (find_path == NULL)
	{
		ft_putstr_fd(cmd->name, 2);
		ft_putstr_fd(": command not found\n", 2);
		return (fake_exit(shell, 127));
	}
	if (execve(find_path, cmd->argv, envp) == -1)
	{
		free(find_path);
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->name, 2);
		ft_putstr_fd("cannot execute binary file\n", 2);
		return (fake_exit(shell, 126));
	}
	return (0);
}

int	execute_signal_cmd(t_shell *shell, t_command *cmds, char *envp[], int *status)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork failed");
		return (1);
	}
	if (pid == 0)
	{
		if (cmds->has_redirects)
			open_files(shell, cmds);
		redirection(0, 0, cmds, 1);
		close_fd(cmds, 0, 1);
		execute(shell, cmds, envp, 1);
		return (fake_exit(shell, 0));
	}
	waitpid(pid, status, 0);
	return (0);
}

int	excute_one_builtin(t_shell *shell, t_command *cmds,
	char **envp, int *status)
{
	int	oldstout;

	oldstout = dup(STDOUT_FILENO);
	if (oldstout == -1)
		exit (1);
	if (cmds->has_redirects)
		open_files(shell, cmds);
	redirection(0, 0, cmds, 1);
	close_fd(cmds, 0, 1);
	*status = execve_builtin(shell, cmds, envp, 1);
	if (dup2(oldstout, STDOUT_FILENO) == -1)
		exit (1);
	close(oldstout);
	return (*status);
}

int	execute_cmds(t_shell *shell, t_command *cmds, char *envp[], size_t cmdcount)
{
	int	status;

	status = 0;
	if (cmdcount == 1)
	{
		if (is_builtins(cmds))
			return (excute_one_builtin(shell, cmds, envp, &status));
		else
			execute_signal_cmd(shell, cmds, envp, &status);
	}
	else
		pipes(shell, cmds, envp, cmdcount, &status);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}