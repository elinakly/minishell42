/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eklymova <eklymova@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 18:16:34 by eklymova          #+#    #+#             */
/*   Updated: 2025/05/10 18:04:34 by eklymova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTE_H
# define EXECUTE_H

# define _GNU_SOURCE
#include "minishell.h"

void		set_signal(void);
int			get_history(void);
void		signal_handler(int sig);
void		signal_handler_child(int sig);
int			history(char *line);
int			execute_cmds(t_shell *shell, t_command *cmds, char *envp[], size_t cmdcount);
char		*find_valid_path(t_shell *shell, const char *com, char **envp);


#endif

