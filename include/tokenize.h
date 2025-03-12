/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschippe <mschippe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 14:15:14 by Mika Schipp       #+#    #+#             */
/*   Updated: 2025/03/12 15:56:10 by mschippe         ###   ########.fr       */
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
# include "variable.h"

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
	MC_VARIABLE		= '$',
	MC_ARG_SEPARATE	= ' '
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
	TT_OUTFILE
}	e_token_type;

typedef enum e_redir_type
{
	RE_OUTPUT_TRUNC,
	RE_OUTPUT_APPEND,
	RE_INPUT,
	RE_HEREDOC
}	e_redir_type;

typedef struct t_part_var
{
	char		*name;
	e_metachar	in_quote_type;
}				t_part_var;

typedef struct t_env_var
{
	char	*name;
	char	*value;
	e_metachar	quote_type;
}			t_env_var;

typedef struct t_triple_index
{
	size_t	var;
	size_t	cmd;
	size_t	res;
}			t_triple_index;

typedef struct t_token
{
	e_token_type	type;
	char			*value;
	bool			has_vars;
	t_part_var			**var_names;
}	t_token;
#endif
