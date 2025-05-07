/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mika <mika@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 13:50:36 by eklymova          #+#    #+#             */
/*   Updated: 2025/05/07 10:49:01 by mika             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

# include "../lib/libft/libft.h"
# include <unistd.h>
# include <stdio.h>
# include <sys/wait.h>
# include <limits.h>
# include <fcntl.h>
# include <errno.h>
# include <linux/limits.h>
# include "structbuild.h"

int			execute(t_shell *shell, t_command *cmd, char **envp, size_t cmdcount);
int			error(int status);
void		create_pipes(t_shell *shell, int num_cmds, int **pipes);
void		free_arr(char **arr);
void		close_fd(t_command *commands, int **pipes, size_t cmdscount);

#endif