/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschippe <mschippe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 13:34:41 by eklymova          #+#    #+#             */
/*   Updated: 2025/05/20 18:11:16 by mschippe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_fd(t_shell *shell, t_command *commands, int **pipes)
{
	int	i;

	i = 0;
	while (i < shell->cmds_count - 1)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}
}

int	error(t_shell *shell, int status)
{
	total_cleanup(shell);
	exit(status);
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

//TODO: resolve_exec_path == SHOULD be freed by execute already but make sure
char	*find_valid_path(t_shell *shell, const char *com, char **envp)
{
	int		i;
	char	**paths;
	char	path[PATH_MAX];

	i = 0;
	if (ft_strchr(com, '/'))
		return (resolve_exec_path(shell, (char *)com));
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

void	wait_pid(t_command *cmds, int *status)
{
	t_command	*commands;
	int			temp_status;

	commands = cmds;
	temp_status = 0;
	while (commands)
	{
		waitpid(commands->pid, &temp_status, 0);
		if (commands->next == NULL)
			*status = temp_status;
		commands = commands->next;
	}
}
