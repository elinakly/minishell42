/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structbuild.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mika <mika@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 00:17:13 by Mika Schipp       #+#    #+#             */
/*   Updated: 2025/05/21 04:21:15 by mika             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTBUILD_H
# define STRUCTBUILD_H
# include "tokenize.h"

t_env_var		**get_vars_from_cmd(char *cmd, t_shell *shell,
					bool track_quotes);
t_token			*get_tokens_from_cmd(char *cmd, t_env_var **vars,
					size_t *tokencount);
t_command		*make_cmd_list(t_token *token, size_t argv_i);
t_parse_result	parse_commands(t_shell *shell, t_command **cmd);
t_token			*make_token(char *raw_token, t_token_type type);
t_redir_type	get_redir_type(t_token *token);
size_t			get_cmd_argc(t_token *token);
bool			insert_redir_in_cmd(t_command *cmd, t_token *token);
bool			insert_into_command(t_command *cmd,
					t_token *token, size_t *argv_i);
t_redirect		*create_redir(t_token *token);
#endif