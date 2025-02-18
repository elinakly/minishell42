/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tokenize.h                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: Mika Schipper <mschippe@student.codam.n      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/02/17 14:15:14 by Mika Schipp   #+#    #+#                 */
/*   Updated: 2025/02/18 01:12:29 by Mika Schipp   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/**
 * TODO: Organize this into different files, probably need one specifically for
 * quotes etc, but that is for another day
 */

typedef enum e_quote_type
{
	QUOTE_NONE,
	QUOTE_SINGLE,
	QUOTE_DOUBLE
}	e_quote_type;

typedef enum e_metachar
{
	MC_NONE			= 0,
	MC_DQUOTE		= '"',
	MC_SQUOTE		= '\'',
	MC_PIPE			= '|',
	MC_REDIR_OUT	= '>',
	MC_REDIR_IN		= '<',
	MC_ESCAPE		= '\\',
	MC_VARIABLE		= '$'
}	e_metachar;
