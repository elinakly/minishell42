/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   path.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: Mika Schipper <mschippe@student.codam.n      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/04/05 22:39:00 by Mika Schipp   #+#    #+#                 */
/*   Updated: 2025/04/05 23:50:53 by Mika Schipp   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/path.h"
#include "../../../include/memory.h"
#include "../../../lib/libft/libft.h"
#include <unistd.h>
#include <stdlib.h>

t_path	*make_path(char *name, t_path *prev, t_path *next)
{
	t_path	*path;

	path = malloc(sizeof(t_path));
	if (!path)
		return (NULL);
	path->name = name;
	path->prev = prev;
	path->next = next;
	return (path);
}

char	**get_split_path(void)
{
	char	*cwd;
	char	*splitpath;
	
	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (NULL);
	splitpath = ft_split(cwd, '/');
	free(cwd);
	return (splitpath);
}

t_path	*get_cwd(void)
{
	t_path	*res;
	t_path	*temp_res;
	char	**splitpath;

	res = NULL;
	splitpath = get_split_path();
	if (!splitpath)
		return (NULL);
	while (*splitpath)
	{
		temp_res = make_path(*splitpath, res, NULL);
		if (!temp_res)
			return (NULL); //TODO: Make free PATH function, currently this leaks
		if (res)
			res->next = temp_res;
		res = temp_res;
		splitpath++;
	}
	return (res);
}

t_path	*getfirstpath(t_path *path)
{
	if (!path)
		return (NULL);
	while (path->prev)
		path = path->prev;
	return (path);
}

t_path	*getlastpath(t_path *path)
{
	if (!path)
		return (NULL);
	while (path->next)
		path = path->next;
	return (path);
}

int	calc_path_len(t_path *path)
{
	int	slashes;
	int	charcount;

	slashes = 0;
	charcount = 0;
	while (path)
	{
		if (path->name)
		{
			charcount += ft_strlen(path->name);
			if (path->next)
				slashes++;
		}
		path = path->next;
	}
	return (charcount + slashes + 1);
}

char	*topathstring(t_path *path, bool from_start)
{
	if (!path)
		return (ft_strdup(""));
	if (from_start)
		path = getfirstpath(path);
	// TODO: Finish this lol
}
