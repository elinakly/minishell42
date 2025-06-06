/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eklymova <eklymova@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 13:50:36 by eklymova          #+#    #+#             */
/*   Updated: 2025/05/24 16:42:42 by eklymova         ###   ########.fr       */
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
# include "minishell.h"

int			execute(t_shell *shell, t_command *cmd, char **envp);
int			error(t_shell *shell, int status);
void		create_pipes(t_shell *shell, int **pipes);
void		free_arr(char **arr);
void		close_fd(t_shell *shell, int **pipes);
int			open_in_files(t_redirect	*redirects);
int			open_out_files(t_redirect	*redirects);
int			open_files(t_shell *shell, t_command *commands);
void		redirects_files(t_shell *shell, t_command *commands,
				bool *has_in, bool *has_out);
void		redirection(t_shell *shell, int i,
				int **pipes, t_command *commands);
int			execute_cmds(t_shell *shell, t_command *cmds, char *envp[]);
int			execute_one_builtin(t_shell *shell, t_command *cmds,
				char **envp, int *status);
int			execute_single_cmd(t_shell *shell, t_command *cmds,
				char *envp[], int *status);
bool		pipes(t_shell *shell, t_command *cmds, int *status);
void		wait_pid(t_command *cmds, int *status);

#endif