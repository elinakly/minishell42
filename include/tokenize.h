/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tokenize.h                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: mschippe <mschippe@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/02/17 14:15:14 by Mika Schipp   #+#    #+#                 */
/*   Updated: 2025/04/16 12:43:16 by Mika Schipp   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZE_H
# define TOKENIZE_H
# define HEREDOC_SPECIAL_CHARS "+-/%@_.:"
# include "minishell.h"

char			**tokenize(char *entry, t_env_var **vars, size_t *amount);
char			*sanitize_token(char *token);
e_token_type	get_token_type(char *raw_token, e_token_type last,
								bool *cmdfound);
size_t			count_esc_metas(t_env_var *var);
bool			is_quote_char(char c, e_quote_type *type);
bool			heredoc_should_expand(char *rawdelim);
bool			is_wspace(char c);
#endif
