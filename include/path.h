/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   path.h                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: Mika Schipper <mschippe@student.codam.n      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/04/05 22:39:39 by Mika Schipp   #+#    #+#                 */
/*   Updated: 2025/04/07 14:15:01 by Mika Schipp   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PATH_H
# define PATH_H
# include <stdbool.h>

typedef struct t_path t_path;

struct t_path
{
	char	*name;
	t_path	*prev;
	t_path	*next;
};

t_path	*get_cwd(void);
t_path	*getfirstpath(t_path *path);
t_path	*getlastpath(t_path *path);
char	*topathstring(t_path *path, bool from_start);
#endif