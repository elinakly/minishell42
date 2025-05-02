/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eklymova <eklymova@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 16:35:59 by eklymova          #+#    #+#             */
/*   Updated: 2025/04/29 16:20:03 by eklymova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "minishell.h"
# include "libft.h"

int		echo(char **args);
int		env(char **envp);
int		pwd(void);
int		ft_exit(char **argv, size_t cmdcount);
bool	is_builtins(t_command *cmds);
int		execve_builtin(t_shell shell, t_command *cmds,
			char **envp, size_t cmdcount);
bool	strequals(char *one, char *two);
int		cd(t_shell shell, t_command *cmd);
int		export(t_shell shell, t_command *cmds, char **envp);

#endif