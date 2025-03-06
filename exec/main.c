/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eklymova <eklymova@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 20:11:57 by eklymova          #+#    #+#             */
/*   Updated: 2025/03/06 15:28:48 by eklymova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <linux/limits.h>

int	ft_strlen(const char *s)
{
	int	lenth;

	lenth = 0;
	while (s[lenth] != '\0')
	{
		lenth++;
	}
	return (lenth);
}

int	ft_strncmp(char *s1, char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while ((s1[i] != '\0' || s2[i] != '\0') && i < n)
	{
		if (s1[i] != s2[i])
			return (s1[i] - s2[i]);
		i++;
	}
	return (0);
}

int	ft_strlcat(char *dst, char *src, int size)
{
	int	dstlen;
	int	srclen;
	int	i;

	srclen = ft_strlen(src);
	dstlen = ft_strlen(dst);
	if (size <= dstlen)
		return (size + srclen);
	i = 0;
	while (src[i] != '\0' && (dstlen + i) < size - 1)
	{
		dst[dstlen + i] = src[i];
		i++;
	}
	dst[dstlen + i] = '\0';
	return (dstlen + srclen);
}

int	ft_strlcpy(char *dst, char *src, int size)
{
	int	i;

	i = 0;
	if (size == 0)
		return (ft_strlen(src));
	while (i < size - 1 && src[i] != '\0')
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (ft_strlen(src));
}

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
	return ("");
}

int	comp_name_len(char **envp)
{
	char	*sesmgr;
	int		len;

	sesmgr = get_value(envp, "SESSION_MANAGER=local/");
	len = 0;
	if (!sesmgr)
		return (0);
	while (sesmgr[len] != '.' && sesmgr[len])
		len++;
	return (len);
}

char	*ft_readline(char **envp)
{
	int		i;
	char	*line;
	char	prompt[PATH_MAX];
	char	*home;
	int		home_len;

	i = 0;
	home = get_value(envp, "HOME=");
	home_len = ft_strlen(home);
	while (envp[i])
	{
		ft_strlcpy(prompt, get_value(envp, "LOGNAME="), PATH_MAX);
		ft_strlcat(prompt, "@", PATH_MAX);
		ft_strlcat(prompt, get_value(envp, "SESSION_MANAGER=local/"),
			ft_strlen(prompt) + comp_name_len(envp) + 1);
		ft_strlcat(prompt, ":~", PATH_MAX);
		ft_strlcat(prompt, get_value(envp, "PWD=") + home_len, PATH_MAX);
		ft_strlcat(prompt, "$ ", PATH_MAX);
		i++;
	}
	line = readline(prompt);
	return (line);
}

// int	main(int argc, char **argv, char **envp)
// {
// 	while (1)
// 		ft_readline(envp);
// 	return (0);
// }