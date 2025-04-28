/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   path.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: mschippe <mschippe@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/04/05 22:39:00 by Mika Schipp   #+#    #+#                 */
/*   Updated: 2025/04/28 16:38:39 by Mika Schipp   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"
#include "../../../include/path.h"
#include "../../../include/venv.h"
#include "../../../include/memory.h"
#include "../../../lib/libft/libft.h"

/**
 * Creates a singular path part
 * Makes a copy of name which must be freed
 * @param name The name of the path part
 * @param prev The previous path part (nullable)
 * @param next The next path part (nullable)
 * @returns A part of a path
 */
t_path	*make_path(char *name, t_path *prev, t_path *next)
{
	t_path	*path;

	if (!name)
		return (NULL);
	path = malloc(sizeof(t_path));
	if (!path)
		return (NULL);
	path->name = ft_strdup(name);
	if (!path->name)
		return (free(path), NULL);
	path->prev = prev;
	path->next = next;
	return (path);
}

/**
 * Wrapper function for splitting a path string into parts using ft_split
 * @param str The path string to split, uses getcwd if NULL
 * @returns An array of strings containing path parts
 */
char	**get_split_path(char *str)
{
	char	**splitpath;
	bool	was_cwd;
	
	was_cwd = false;
	if (!str)
	{
		str = getcwd(NULL, 0);
		if (!str)
			return (NULL);
		was_cwd = true;
	}
	splitpath = ft_split(str, '/');
	if (was_cwd)
		free(str);
	return (splitpath);
}

/**
 * Gets a linkedlist of path parts representing the current working directory
 * @returns A linkedlist of path parts
 */
t_path	*get_cwd(void)
{
	t_path	*root;
	t_path	*temp_res;
	char	**parts;
	size_t	partsindex;

	partsindex = 0;
	root = make_path("/", NULL, NULL);
	if (!root)
		return (NULL);
	parts = get_split_path(NULL);
	if (!parts)
		return (root);
	while (parts[partsindex])
	{
		temp_res = make_path(parts[partsindex++], root, NULL);
		if (!temp_res)
			return (free_array((void **)parts, NULL),
					free_path(root, true), NULL);
		root->next = temp_res;
		root = temp_res;
	}
	return (free_array((void **)parts, NULL), root);
}

/**
 * Gets the home directory from environment
 */
char	*get_homedir(t_shell shell)
{
	char	*homedir;
	
	homedir = get_env_val(shell, "HOME");
	if (!homedir)
		return (ft_strdup("/"));
}

/**
 * Expands ~ to home directory if the path starts with ~
 */
char	*expand_dir(t_shell shell, char *dir)
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
char	*resolve_exec_path(t_shell shell, char *path)
{
	char	*cwd;
	char	*exp;
	char	*res;
	size_t	index;
	size_t total;

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

/**
 * Returns the first path part in a path linkedlist
 * @param path Any path part
 * @returns The first path part
 */
t_path	*getfirstpath(t_path *path)
{
	if (!path)
		return (NULL);
	while (path->prev)
		path = path->prev;
	return (path);
}

/**
 * Returns the last path part in a path linkedlist
 * @param path Any path part
 * @returns The last path part
 */
t_path	*getlastpath(t_path *path)
{
	if (!path)
		return (NULL);
	while (path->next)
		path = path->next;
	return (path);
}

/**
 * Calculates what the length of a path string will be
 * Does not necessarily start at the start of a path
 * @param path The path part from which to start counting
 * @returns The length of what the path string would be for this path
 */
size_t	calc_path_len(t_path *path)
{
	size_t	slashes;
	size_t	charcount;

	slashes = 1;
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

/**
 * Converts a path part linkedlist to a path string
 * @param path Any path part to start from
 * @param from_start Whether to use full path
 * @returns A path string
 */
char	*topathstring(t_path *path, bool from_start)
{
	size_t		len;
	size_t		resindex;
	size_t		nameindex;
	char	*res;
	
	if (!path)
		return (ft_strdup("/"));
	if (from_start)
		path = getfirstpath(path);
	len = calc_path_len(path);
	res = ft_calloc(len, sizeof(char));
	if (!res)
		return (NULL);
	nameindex = 0;
	resindex = 0;
	while (path)
	{
		while (path->name[nameindex++])
		res[resindex++] = path->name[nameindex - 1];
		if (path->next && path->prev)
			res[resindex++] = '/';
		path = path->next;
		nameindex = 0;
	}
	return (res);
}

/**
 * Goes back 1 folder in the current path, frees everything that comes after
 * @param path The path from which to go back 1 folder
 */
void parent_dir(t_path **path)
{
	t_path	*curr;
	t_path	*prev;

	curr = *path;
	prev = curr->prev;
	if (!prev)
		return ;
	prev->next = NULL;
	*path = prev;
	free_path(curr, false);
}