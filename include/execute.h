/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschippe <mschippe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 18:16:34 by eklymova          #+#    #+#             */
/*   Updated: 2025/05/24 16:49:26 by mschippe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTE_H
# define EXECUTE_H

# define _GNU_SOURCE
# include "minishell.h"

int			get_history(t_shell *shell);
int			history(t_shell *shell, char *line);
int			execute_cmds(t_shell *shell, t_command *cmds, char *envp[]);
char		*find_valid_path(t_shell *shell, const char *com, char **envp);
void		total_cleanup(t_shell *shell);
char		*ft_readline(t_shell *shell, char **envp);
size_t		ft_cmdcount(t_command *head);
void		errors(char *msg);

#endif
