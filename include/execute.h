/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   execute.h                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: eklymova <eklymova@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/03/12 18:16:34 by eklymova      #+#    #+#                 */
/*   Updated: 2025/03/22 10:03:39 by Mika Schipp   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTE_H
# define EXECUTE_H

# define _GNU_SOURCE
# include <signal.h>
# include <unistd.h>
#include <readline/readline.h>
#include "structbuild.h"

void		set_signal(void);
int			get_history(void);
int			history(char *line);
int			execute_cmds(t_command *cmds, char *envp[], size_t cmdcount);
static char	*find_valid_path(const char *com, char **envp);


#endif

