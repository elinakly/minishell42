/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   enums.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: Mika Schipper <mschippe@student.codam.n      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/04/16 12:46:56 by Mika Schipp   #+#    #+#                 */
/*   Updated: 2025/04/16 12:47:11 by Mika Schipp   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENUMS_H
# define ENUMS_H

typedef enum e_quote_type
{
	QUOTE_NONE,
	QUOTE_SINGLE,
	QUOTE_DOUBLE,
}	e_quote_type;

typedef enum e_redir_type
{
	RE_UNKNOWN,
	RE_OUTPUT_TRUNC,
	RE_OUTPUT_APPEND,
	RE_INPUT,
	RE_HEREDOC,
}	e_redir_type;

typedef enum e_parse_result
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
	NONE,
}	e_parse_result;

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
	MC_SEPAR_SPACE	= ' ',
	MC_SEPAR_TAB	= '\t'
}	e_metachar;

typedef enum e_token_type
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
}	e_token_type;

#endif