/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eklymova <eklymova@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 22:39:00 by Mika Schipp       #+#    #+#             */
/*   Updated: 2025/05/09 17:39:50 by eklymova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"
#include "../../../include/path.h"
#include "../../../include/venv.h"
#include "../../../include/memory.h"
#include "../../../lib/libft/libft.h"

/**
 * Gets the home directory from environment
 */
char	*get_homedir(t_shell *shell)
{
	char	*homedir;

	homedir = get_env_val(shell, "HOME");
	if (!homedir)
		return (ft_strdup("/"));
	return (homedir);
}

/**
 * Expands ~ to home directory if the path starts with ~
 */
char	*expand_dir(t_shell *shell, char *dir)
{
	char	*dircpy;
	char	*home;
	char	*res;

	if (!dir)
		return (NULL);
	dircpy = ft_strdup(dir);
	if (!dircpy)
		return (NULL);
	if (dircpy[0] == '~' && (dircpy[1] == '/' || !dircpy[1]))
	{
		home = get_homedir(shell);
		if (!home)
			return (free(dircpy), NULL);
		res = ft_strjoin(home, dircpy + 1);
		return (free(dircpy), free(home), res);
	}
	return (dircpy);
}

/**
 * Joins two strings together with a slash in between
 * Destination needs to be malloced already
 */
void	join_with_slash(char *cwd, char *add, char *dest)
{
	size_t	index;
	size_t	total;

	index = 0;
	total = 0;
	while (cwd[index])
		dest[total++] = cwd[index++];
	index = 0;
	dest[total++] = '/';
	while (add[index])
		dest[total++] = add[index++];
	dest[total] = '\0';
}

/**
 * Abuse temporary chdir in combination with getcwd to
 * easily create a full path to something
 */
char	*resolve_exec_path(t_shell *shell, char *path)
{
	char	*cwd;
	char	*exp;
	char	*res;
	size_t	index;
	size_t	total;

	exp = expand_dir(shell, path);
	index = 0;
	total = 0;
	if (!exp)
		return (NULL);
	if (exp[0] == '/')
		return (exp);
	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (free(exp), NULL);
	res = malloc((ft_strlen(exp) + ft_strlen(cwd) + 2) * sizeof(char));
	if (!res)
		return (free(exp), free(cwd), NULL);
	join_with_slash(cwd, exp, res);
	return (res);
}
