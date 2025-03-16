/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tokenize.h                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: mschippe <mschippe@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/02/17 14:15:14 by Mika Schipp   #+#    #+#                 */
/*   Updated: 2025/03/16 11:48:57 by Mika Schipp   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/**
 * TODO: Organize this into different files, probably need one specifically for
 * quotes etc, but that is for another day
 * TODO: IFNDEF!!!!!!!!!!!
 */
#ifndef TOKENIZE_H
# define TOKENIZE_H
# include <stdlib.h>
# include <stdbool.h>
#include "tokenandvar.h"
#include "variable.h"


typedef enum e_quote_type
{
	QUOTE_NONE,
	QUOTE_SINGLE,
	QUOTE_DOUBLE
}	e_quote_type;

typedef enum e_redir_type
{
	RE_OUTPUT_TRUNC,
	RE_OUTPUT_APPEND,
	RE_INPUT,
	RE_HEREDOC
}	e_redir_type;

char	**tokenize(char *entry, t_env_var **vars, size_t *amount);
char	*sanitize_token(char *token);
e_token_type	get_token_type(char *raw_token, e_token_type last,
								bool *cmdfound);
size_t	count_esc_metas(t_env_var *var);
#endif
