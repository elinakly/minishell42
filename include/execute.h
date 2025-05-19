/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mika <mika@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 18:16:34 by eklymova          #+#    #+#             */
/*   Updated: 2025/05/19 21:02:01 by mika             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTE_H
# define EXECUTE_H

# define _GNU_SOURCE
# include "minishell.h"

int			get_history(void);
int			history(char *line);
int			execute_cmds(t_shell *shell, t_command *cmds, char *envp[]);
char		*find_valid_path(t_shell *shell, const char *com, char **envp);
void		total_cleanup(t_shell *shell);

#endif
