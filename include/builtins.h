/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mika <mika@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 16:35:59 by eklymova          #+#    #+#             */
/*   Updated: 2025/05/07 10:48:26 by mika             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H
# define CD_TM_ARGS "minishell: cd: too many arguments\n"

# include "minishell.h"
# include "libft.h"

int		echo(char **args);
int		env(char **envp);
int		pwd(void);
int		ft_exit(t_shell *shell, char **argv, size_t cmdcount);
bool	is_builtins(t_command *cmds);
int		execve_builtin(t_shell *shell, t_command *cmds,
			char **envp, size_t cmdcount);
bool	strequals(char *one, char *two);
int		cd(t_shell *shell, t_command *cmd);
int		export(t_shell *shell, t_command *cmds, char **envp);
int		fake_exit(t_shell *shell, int code);

#endif