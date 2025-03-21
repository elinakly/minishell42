/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eklymova <eklymova@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 13:34:41 by eklymova          #+#    #+#             */
/*   Updated: 2025/03/19 15:33:10 by eklymova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

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

void	execute(char *com, char **argv, char **envp)
{
	char	*find_path;

	find_path = find_valid_path(com, envp);
	if (com == NULL)
	{
		free_arr(argv);
		exit(error(3));
	}
	if (find_path == NULL)
	{
		free_arr(argv);
		exit(error(127));
	}
	if (execve(find_path, argv, envp) == -1)
	{
		free(find_path);
		free_arr(argv);
		exit(error(126));
	}
}
