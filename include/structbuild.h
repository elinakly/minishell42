/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structbuild.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eklymova <eklymova@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 00:17:13 by Mika Schipp       #+#    #+#             */
/*   Updated: 2025/05/13 17:00:05 by eklymova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTBUILD_H
# define STRUCTBUILD_H
# include "tokenize.h"

t_env_var		**get_vars_from_cmd(char *cmd, t_shell *shell,
					bool track_quotes);
t_token			*get_tokens_from_cmd(char *cmd, t_env_var **vars,
					size_t *tokencount);
t_command		*make_cmd_list(t_token *token);
e_parse_result	parse_commands(t_shell *shell, t_command **cmd);
t_token			*make_token(char *raw_token, e_token_type type);
e_redir_type	get_redir_type(t_token *token);
size_t			get_cmd_argc(t_token *token);
bool			insert_redir_in_cmd(t_command *cmd, t_token *token);
bool			insert_into_command(t_command *cmd,
					t_token *token, size_t *argv_i);
t_redirect		*create_redir(t_token *token);
#endif