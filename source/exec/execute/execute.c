/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eklymova <eklymova@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 20:03:06 by eklymova          #+#    #+#             */
/*   Updated: 2025/05/24 16:47:32 by eklymova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute(t_shell *shell, t_command *cmd, char **envp)
{
	char	*find_path;
	int		status;

	if (is_builtins(cmd))
	{
		status = execve_builtin(shell, cmd, envp);
		total_cleanup(shell);
		exit(status);
	}
	find_path = find_valid_path(shell, cmd->name, envp);
	if (cmd->name == NULL)
		return (not_so_fake_exit(shell, error(shell, 3)));
	if (find_path == NULL)
	{
		ft_putstr_fd("Command not found\n", 2);
		return (not_so_fake_exit(shell, error(shell, 127)));
	}
	if (execve(find_path, cmd->argv, envp) == -1)
	{
		ft_putstr_fd("Cannot execute binary file\n", 2);
		return (free(find_path), not_so_fake_exit(shell, error(shell, 127)));
	}
	return (0);
}

int	child_handler(t_shell *shell, t_command *cmds, char *envp[], int *status)
{
	set_child_default_signal();
	if (cmds->has_redirects)
	{
		*status = open_files(shell, cmds);
		if (*status != 1)
			return (1);
	}
	redirection(shell, 0, 0, cmds);
	close_fd(shell, 0);
	if (cmds->has_command)
		execute(shell, cmds, envp);
	return (not_so_fake_exit(shell, 0));
}

int	execute_single_cmd(t_shell *shell, t_command *cmds,
		char *envp[], int *status)
{
	pid_t	pid;

	set_ignore_signal();
	pid = fork();
	if (pid == -1)
		return (perror("fork failed"), 1);
	if (pid == 0)
		return (child_handler(shell, cmds, envp, status));
	waitpid(pid, status, 0);
	if (WIFSIGNALED(*status))
	{
		if (WTERMSIG(*status) == SIGQUIT)
			ft_putstr_fd("Quit (core dumped)\n", 2);
		else if (WTERMSIG(*status) == SIGINT)
			ft_putchar_fd('\n', 2);
	}
	return (0);
}

int	execute_one_builtin(t_shell *shell, t_command *cmds,
	char **envp, int *status)
{
	int	oldstout;

	oldstout = dup(STDOUT_FILENO);
	if (oldstout == -1)
		return (not_so_fake_exit(shell, 1));
	if (cmds->has_redirects)
	{
		*status = open_files(shell, cmds);
		if (*status != 1)
			return (*status);
	}
	redirection(shell, 0, 0, cmds);
	close_fd(shell, 0);
	*status = execve_builtin(shell, cmds, envp);
	if (dup2(oldstout, STDOUT_FILENO) == -1)
		return (not_so_fake_exit(shell, 1));
	close(oldstout);
	return (*status);
}

int	execute_cmds(t_shell *shell, t_command *cmds, char *envp[])
{
	int	status;

	status = 0;
	shell->cmds_count = ft_cmdcount(cmds);
	if (shell->cmds_count == 1)
	{
		if (is_builtins(cmds))
			return (execute_one_builtin(shell, cmds, envp, &status));
		else
			execute_single_cmd(shell, cmds, envp, &status);
	}
	else
	{
		if (!pipes(shell, cmds, &status))
			return (1);
	}
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}
