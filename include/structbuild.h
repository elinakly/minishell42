/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structbuild.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mika <mika@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 00:17:13 by Mika Schipp       #+#    #+#             */
/*   Updated: 2025/05/08 13:44:09 by mika             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTBUILD_H
# define STRUCTBUILD_H
# include "tokenize.h"

t_env_var		**get_vars_from_cmd(char *cmd, t_shell *shell, bool track_quotes);
t_token			*get_tokens_from_cmd(char *cmd, t_env_var **vars, size_t *tokencount);
t_command		*make_cmd_list(t_token *token);
e_parse_result	parse_commands(t_shell *shell, t_command **cmd);
#endif