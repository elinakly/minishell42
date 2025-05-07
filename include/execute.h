/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mika <mika@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 18:16:34 by eklymova          #+#    #+#             */
/*   Updated: 2025/05/07 10:48:31 by mika             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTE_H
# define EXECUTE_H

# define _GNU_SOURCE
#include "minishell.h"

void		set_signal(void);
int			get_history(void);
void		signal_handler(int sig);
int			history(char *line);
int			execute_cmds(t_shell *shell, t_command *cmds, char *envp[], size_t cmdcount);
static char	*find_valid_path(t_shell *shell, const char *com, char **envp);


#endif

