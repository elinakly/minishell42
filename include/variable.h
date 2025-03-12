/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   variable.h                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: Mika Schipper <mschippe@student.codam.n      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/03/09 13:49:41 by Mika Schipp   #+#    #+#                 */
/*   Updated: 2025/03/12 04:08:23 by Mika Schipp   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef VARIABLE_H
# define VARIABLE_H
# include "tokenize.h"

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
#endif
