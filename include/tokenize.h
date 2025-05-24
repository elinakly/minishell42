/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eklymova <eklymova@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 14:15:14 by Mika Schipp       #+#    #+#             */
/*   Updated: 2025/05/24 16:44:58 by eklymova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZE_H
# define TOKENIZE_H
# define HEREDOC_SPECIAL_CHARS "+-/%@_.:"
# include "minishell.h"

char			**tokenize(char *entry, t_env_var **vars, size_t *amount);
char			*sanitize_token(char *token);
t_token_type	get_token_type(char *raw_token, t_token_type last,
					bool *cmdfound);
t_token_type	get_token_type2(t_token_type last, bool *cmdfound);
size_t			count_esc_metas(t_env_var *var);
bool			is_quote_char(char c, t_quote_type *type);
bool			heredoc_should_expand(char *rawdelim);
bool			is_wspace(char c);
bool			is_env_var_char(char c);
size_t			token_size(char *str, bool include_spaces);
size_t			count_tokens(char *entry);
bool			can_escape(char c, t_metachar quot);
size_t			calc_decrease(char *token);
void			clear_env_var(void *var);
void			clear_part_var(void *var);
bool			is_escaped_char(char *str, int index);
bool			disrupts_token(t_metachar meta);
size_t			skip_meta(char *str);
size_t			skip_quoted(char *str);
size_t			skip_spaces(char *str);
#endif
