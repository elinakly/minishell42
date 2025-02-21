/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tokenize.h                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: Mika Schipper <mschippe@student.codam.n      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/02/17 14:15:14 by Mika Schipp   #+#    #+#                 */
/*   Updated: 2025/02/21 00:38:18 by Mika Schipp   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/**
 * TODO: Organize this into different files, probably need one specifically for
 * quotes etc, but that is for another day
 * TODO: IFNDEF!!!!!!!!!!!
 */

#include <stdlib.h>
#include <stdbool.h>

typedef enum e_quote_type
{
	QUOTE_NONE,
	QUOTE_SINGLE,
	QUOTE_DOUBLE
}	e_quote_type;

typedef enum e_metachar
{
	MC_ERROR		= -1,
	MC_NONE			= 0,
	MC_DQUOTE		= '"',
	MC_SQUOTE		= '\'',
	MC_PIPE			= '|',
	MC_REDIR_OUT	= '>',
	MC_REDIR_IN		= '<',
	MC_ESCAPE		= '\\',
	MC_VARIABLE		= '$'
}	e_metachar;

typedef enum e_token_type
{
	TT_UNKNOWN,
	TT_COMMAND,
	TT_ARGUMENT,
	TT_HEREDOC,
	TT_PIPE,
	TT_RE_IN,
	TT_RE_OUT,
	TT_RE_OUT_APPEND,
	TT_INFILE,
	TT_OUTFILE,
}	e_token_type;

typedef struct t_token
{
	e_token_type	type;
	char			*value;
	bool			has_vars;
	size_t			**var_indices;
	char			**var_names;
}	t_token;
