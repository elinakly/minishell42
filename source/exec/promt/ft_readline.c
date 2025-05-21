/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_readline.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eklymova <eklymova@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 20:11:57 by eklymova          #+#    #+#             */
/*   Updated: 2025/05/21 18:14:39 by eklymova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

bool strcontainsstr(char *str, char *sub)
{
	int	i;
	int	j;

	i = 0;
	while (str[i])
	{
		j = 0;
		while (str[i + j] && str[i + j] == sub[j])
			j++;
		if (!sub[j])
			return (true);
		i++;
	}
	return (false);
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

bool	build_prompt(char prompt[], char **envp, int home_len)
{
	char	*home;
	char	*pwd;

	home = get_value(envp, "HOME=");
	pwd = get_value(envp, "PWD=");
	if (!home || !pwd || !strcontainsstr(pwd, home))
		home_len = 0;
	if (!(ft_strlcpy(prompt, get_value(envp, "LOGNAME="), PATH_MAX)))
		return (errors("Error: ft_strlcpy in readline failed\n"), false);
	if (!(ft_strlcat(prompt, "@", PATH_MAX)))
		return (errors("Error : strlcat in readline failed"), false);
	if (get_value(envp, "SESSION_MANAGER=local/"))
		ft_strlcat(prompt, get_value(envp, "SESSION_MANAGER=local/"),
			ft_strlen(prompt) + comp_name_len(envp) + 1);
	else
		ft_strlcat(prompt, get_value(envp, "NAME="), PATH_MAX);
	if (home_len > 0)
		ft_strlcat(prompt, ":~", PATH_MAX);
	else
		ft_strlcat(prompt, ":", PATH_MAX);
	if (!ft_strlcat(prompt, get_value(envp, "PWD=") + home_len, PATH_MAX))
		return (errors("Error : strlcat in readline failed"), false);
	if (!ft_strlcat(prompt, "$ ", PATH_MAX))
		return (errors("Error : strlcat in readline failed"), false);
	return (true);
}

char	*ft_readline(t_shell *shell, char **envp)
{
	int		i;
	char	*line;
	char	prompt[PATH_MAX];
	char	*home;
	int		home_len;

	i = 0;
	home = get_value(envp, "HOME=");
	if (!home)
		return (NULL);
	home_len = ft_strlen(home);
	while (envp[i])
	{
		if (!build_prompt(prompt, envp, home_len))
			return (NULL);
		i++;
	}
	line = readline(prompt);
	if (line == 0)
	{
		if (isatty(STDIN_FILENO))
			ft_putstr_fd("exit\n", 2);
		not_so_fake_exit(shell, shell->status);
	}
	return (line);
}
