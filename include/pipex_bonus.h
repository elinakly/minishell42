/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eklymova <eklymova@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 13:50:36 by eklymova          #+#    #+#             */
/*   Updated: 2025/05/07 20:07:06 by eklymova         ###   ########.fr       */
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
int			open_in_files(t_redirect	*redirects);
int			open_out_files(t_redirect	*redirects);
int			open_files(t_shell *shell, t_command *commands);
void		redirects_files(t_command *commands, bool *has_in, bool *has_out);
void		redirection(int i, int **pipes, t_command *commands, size_t cmdcount);
int			execute_cmds(t_shell *shell, t_command *cmds, char *envp[], size_t cmdcount);
int			excute_one_builtin(t_shell *shell, t_command *cmds, char **envp, int *status);
int			execute_signal_cmd(t_shell *shell, t_command *cmds, char *envp[], int *status);
char		*find_valid_path(t_shell *shell, const char *com, char **envp);
int			pipes(t_shell *shell, t_command *cmds, char *envp[], size_t cmdcount, int *status);

#endif