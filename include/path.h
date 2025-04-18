/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   path.h                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: mschippe <mschippe@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/04/05 22:39:39 by Mika Schipp   #+#    #+#                 */
/*   Updated: 2025/04/16 12:40:36 by Mika Schipp   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PATH_H
# define PATH_H
# include "minishell.h"

t_path	*get_cwd(void);
t_path	*getfirstpath(t_path *path);
t_path	*getlastpath(t_path *path);
char	*topathstring(t_path *path, bool from_start);
void	parent_dir(t_path **path);
#endif