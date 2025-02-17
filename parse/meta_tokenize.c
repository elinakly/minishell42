/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   meta_tokenize.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: Mika Schipper <mschippe@student.codam.n      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/02/17 19:41:33 by Mika Schipp   #+#    #+#                 */
/*   Updated: 2025/02/17 20:23:27 by Mika Schipp   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <stdlib.h>
#include "tokenize.h"

bool	is_metachar(char *str, size_t index)
{
	char	c;

	c = *str;
	return ((c == MC_DQUOTE
		|| c == MC_ESCAPE
		|| c == MC_PIPE
		|| c == MC_REDIR_IN
		|| c == MC_REDIR_OUT
		|| c == MC_SQUOTE
		|| c == MC_VARIABLE)
		&& !is_escaped_char(str, index));
}

int	skip_metachar(char *str)
{
	(void)str;
}
