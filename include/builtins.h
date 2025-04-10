/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eklymova <eklymova@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 16:35:59 by eklymova          #+#    #+#             */
/*   Updated: 2025/04/10 20:04:41 by eklymova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include <unistd.h>
# include "../lib/libft/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <linux/limits.h>
# include <fcntl.h>
# include <readline/history.h>
# include "structbuild.h"
#include <sys/wait.h>

int		echo(char **args);
int		env(char **envp);
int		pwd(void);
int		ft_exit(void);
bool	is_builtins(t_command *cmds, char **envp);
void	execve_builtin(t_command *cmds, char **envp);

#endif