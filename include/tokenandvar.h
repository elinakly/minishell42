/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tokenandvar.h                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: Mika Schipper <mschippe@student.codam.n      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/03/13 01:22:00 by Mika Schipp   #+#    #+#                 */
/*   Updated: 2025/03/19 00:52:52 by Mika Schipp   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENANDVAR_H
# define TOKENANDVAR_H

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