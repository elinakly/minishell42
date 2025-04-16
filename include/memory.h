/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   memory.h                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: mschippe <mschippe@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/02/20 01:24:49 by Mika Schipp   #+#    #+#                 */
/*   Updated: 2025/04/16 12:43:41 by Mika Schipp   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MEMORY_H
# define MEMORY_H
# include "minishell.h"

void	free_array(void **arr, void (*del)(void *));
void	clear_env_var(void *var);
void	clear_part_var(void *var);
size_t	ft_arrlen(void **arr);
void	free_tokens(t_token *head);
void	free_commands(t_command *head);
void	free_single_token(t_token *token);
void	free_single_redir(t_redirect *redir);
void	free_single_cmd(t_command *cmd);
void	free_path(t_path *path, bool from_start);
#endif