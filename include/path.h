/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   path.h                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: mschippe <mschippe@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/04/05 22:39:39 by Mika Schipp   #+#    #+#                 */
/*   Updated: 2025/04/28 17:06:06 by Mika Schipp   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PATH_H
# define PATH_H
# include "minishell.h"

char	*get_homedir(t_shell shell);
char	*expand_dir(t_shell shell, char *dir);
char	*resolve_exec_path(t_shell shell, char *path);
#endif