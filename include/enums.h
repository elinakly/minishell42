/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enums.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschippe <mschippe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 12:46:56 by Mika Schipp       #+#    #+#             */
/*   Updated: 2025/05/20 18:16:36 by mschippe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENUMS_H
# define ENUMS_H

typedef enum t_quote_type
{
	QUOTE_NONE,
	QUOTE_SINGLE,
	QUOTE_DOUBLE,
}	t_quote_type;

typedef enum t_redir_type
{
	RE_UNKNOWN,
	RE_OUTPUT_TRUNC,
	RE_OUTPUT_APPEND,
	RE_INPUT,
	RE_HEREDOC,
}	t_redir_type;

typedef enum t_parse_result
{
	UNCLOSED_SQUOTE,
	UNCLOSED_DQUOTE,
	UNFINISHED_PIPE,
	ESCAPED_NEWLINE,
	HEREDOC,
	SYNTAX_ERROR,
	MALLOC_FAIL,
	EMPTY,
	PARSEOK,
	HEREDOC_CANCEL,
	NONE,
}	t_parse_result;

typedef enum t_metachar
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
	MC_SEPAR_SPACE	= ' ',
	MC_SEPAR_TAB	= '\t'
}	t_metachar;

typedef enum t_token_type
{
	TT_UNKNOWN,
	TT_COMMAND,
	TT_ARGUMENT,
	TT_HEREDOC,
	TT_HEREDOC_DELIM,
	TT_PIPE,
	TT_RE_IN,
	TT_RE_OUT,
	TT_RE_OUT_APPEND,
	TT_INFILE,
	TT_OUTFILE
}	t_token_type;

#endif