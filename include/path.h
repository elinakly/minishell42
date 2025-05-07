/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mika <mika@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 22:39:39 by Mika Schipp       #+#    #+#             */
/*   Updated: 2025/05/07 10:48:41 by mika             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PATH_H
# define PATH_H
# include "minishell.h"

char	*get_homedir(t_shell *shell);
char	*expand_dir(t_shell *shell, char *dir);
char	*resolve_exec_path(t_shell *shell, char *path);
#endif