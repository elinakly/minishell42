/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eklymova <eklymova@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 20:11:57 by eklymova          #+#    #+#             */
/*   Updated: 2025/03/04 21:12:02 by eklymova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

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

char	*ft_strjoin(char *s1, char *s2)
{
	char	*arr;
	int		i;
	int		j;

	arr = (char *)malloc((ft_strlen(s1) + ft_strlen(s2) + 1) * sizeof(char));
	if (arr == NULL)
	{
		return (NULL);
	}
	i = 0;
	while (s1[i] != '\0')
	{
		arr[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2[j] != '\0')
	{
		arr[i] = s2[j];
		j++;
		i++;
	}
	arr[i] = '\0';
	return (arr);
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

void ft_readline(char **envp)
{
	int		i = 0;
	char	*line;
	char	*prompt = NULL;
	char	*cwd = NULL;

	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PWD=", 4) == 0)
		{
			cwd = envp[i] + 4;
			break ;
		}
		i++;
	}
	if (cwd)
		prompt = ft_strjoin(cwd, "/minishell$ ");
	line = readline(prompt);
}

int	main(int argc, char **argv, char **envp)
{
	ft_readline(envp);
	return (0);
}