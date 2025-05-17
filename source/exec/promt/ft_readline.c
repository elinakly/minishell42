/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_readline.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschippe <mschippe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 20:11:57 by eklymova          #+#    #+#             */
/*   Updated: 2025/05/16 20:49:32 by mschippe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <linux/limits.h>
#include "../../../lib/libft/libft.h"
#include "../../../include/builtins.h"


void	errors(char *msg);

char	*get_value(char **envp, char *key)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], key, ft_strlen(key)) == 0)
			return (envp[i] + ft_strlen(key));
		i++;
	}
	return (NULL);
}

int	comp_name_len(char **envp)
{
	char	*sesmgr;
	int		len;

	sesmgr = get_value(envp, "SESSION_MANAGER=local/");
	len = 0;
	if (!sesmgr)
		return (0);
	while ((sesmgr[len] != '.' && sesmgr[len] != ':') && sesmgr[len])
		len++;
	return (len);
}

/**
 * TODO: Make prompt not have ~ if we are not in home dir
 */
char	*ft_readline(t_shell *shell, char **envp)
{
	int		i;
	char	*line;
	char	prompt[PATH_MAX];
	char	*home;
	int		home_len;

	i = 0;
	if (!(home = get_value(envp, "HOME=")))
		return (NULL);
	home_len = ft_strlen(home);
	while (envp[i])
	{
		if (!(ft_strlcpy(prompt, get_value(envp, "LOGNAME="), PATH_MAX)))
			return (errors("Error: ft_strlcpy in readline failed\n"), NULL);
		if (!(ft_strlcat(prompt, "@", PATH_MAX)))
			return (errors("Error : strlcat in readline failed"), NULL);
		if (get_value(envp, "SESSION_MANAGER=local/"))
			ft_strlcat(prompt, get_value(envp, "SESSION_MANAGER=local/"),
				ft_strlen(prompt) + comp_name_len(envp) + 1); ///SCHOOL
		else
			ft_strlcat(prompt, get_value(envp, "NAME="), PATH_MAX); //HOME REMOVE
		if (!(ft_strlcat(prompt, ":~", PATH_MAX)))
			return (errors("Error : strlcat in readline failed"), NULL);
		if (!ft_strlcat(prompt, get_value(envp, "PWD=") + home_len, PATH_MAX))
			return (errors("Error : strlcat in readline failed"), NULL);
		if (!ft_strlcat(prompt, "$ ", PATH_MAX))
			return (errors("Error : strlcat in readline failed"), NULL);
		i++;
	}
	line = readline(prompt);
	if (line == 0)
	{
		// if (isatty(STDIN_FILENO))
		// 	ft_putstr_fd("exit\n", 2);
		not_so_fake_exit(shell, shell->last_status);
	}
	return (line);
}
