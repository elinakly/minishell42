/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschippe <mschippe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 13:49:41 by Mika Schipp       #+#    #+#             */
/*   Updated: 2025/05/20 18:16:21 by mschippe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VARIABLE_H
# define VARIABLE_H
# include "minishell.h"

bool		set_quote_state(char *cmd, size_t index, t_metachar *current);
bool		is_var_char(char *str, size_t index);
size_t		skip_var_chars(char *cmd, size_t index);
size_t		get_var_count(char *cmd, bool track_quotes);
size_t		var_len(char *cmd, size_t i, bool track_quotes);
t_part_var	**get_var_names(char *cmd, size_t varcount,
				t_part_var **names, bool t);
t_env_var	*make_var(t_part_var *part, t_shell *shell);
t_env_var	**get_command_vars(t_part_var **names, t_shell *shell);
size_t		calc_expanded_len(char *cmd, t_env_var **vars);
char		*get_escaped_value(t_env_var *var);
bool		insert_var(char *res, t_env_var *var, size_t *index);
char		*get_expanded_cmd(char *cmd, t_env_var **vars, bool track_quotes);
bool		is_meta_var(char *cmd, t_triple_index i, t_metachar *meta,
				t_metachar *quot);
bool		is_meta(char *str, size_t index, t_metachar *meta);
#endif
