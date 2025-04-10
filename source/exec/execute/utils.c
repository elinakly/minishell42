/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eklymova <eklymova@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 13:34:41 by eklymova          #+#    #+#             */
/*   Updated: 2025/04/10 19:36:42 by eklymova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"
#include "builtins.h"

void	close_fd(t_command *commands, int **pipes, size_t cmdscount)
{
	int	i;

	// close(commands.input_fd);
	// close(commands.output_fd);
	i = 0;
	while (i < cmdscount - 1)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}
}

int	error(int status)
{
	if (status == 1)
		perror("Dup2 failed");
	else if (status == 2)
		perror("Path failed");
	else if (status == 3)
		perror("Execve failed");
	else if (status == 126)
		ft_putstr_fd("Error: command not found\n", 2);
	else if (status == 127)
		ft_putstr_fd("Execve failed\n", 2);
	exit (status);
}

void	free_arr(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

static char	*find_valid_path(const char *com, char **envp)
{
	int		i;
	char	**paths;
	char	path[PATH_MAX];

	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5) != 0)
		i++;
	if (!envp[i])
		return (NULL);
	paths = ft_split(envp[i] + 5, ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		ft_strlcpy(path, paths[i], PATH_MAX);
		ft_strlcat(path, "/", PATH_MAX);
		ft_strlcat(path, com, PATH_MAX);
		if (access(path, X_OK) == 0)
			return (free_arr(paths), ft_strdup(path));
		i++;
	}
	return (free_arr(paths), NULL);
}
void	execute(t_command *cmd, char **envp)
{
	char	*find_path;

	if (is_builtins(cmd, envp))
	{
		execve_builtin(cmd, envp);
		exit(EXIT_SUCCESS);
	}
	find_path = find_valid_path(cmd->name, envp);
	if (cmd->name == NULL)
		exit(error(3));
	if (find_path == NULL)
	{
		ft_putstr_fd(cmd->name, 2);
		ft_putstr_fd(": command not found\n", 2);
		exit(127);
	}
	if (execve(find_path, cmd->argv, envp) == -1)
	{
		free(find_path);
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->name, 2);
		ft_putstr_fd("cannot execute binary file\n", 2);
		exit(126);
	}
}
