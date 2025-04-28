/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtins.h                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: eklymova <eklymova@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/03/07 16:35:59 by eklymova      #+#    #+#                 */
/*   Updated: 2025/04/28 14:33:20 by Mika Schipp   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "minishell.h"
# include "libft.h"

int		echo(char **args);
int		env(char **envp);
int		pwd(void);
int		ft_exit(char **argv);
bool	is_builtins(t_command *cmds, char **envp);
void	execve_builtin(t_shell shell, t_command *cmds, char **envp);
bool	strequals(char *one, char *two);
int		cd(t_shell shell, t_command *cmd);

#endif