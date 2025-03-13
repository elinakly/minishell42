/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   structbuild.h                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: Mika Schipper <mschippe@student.codam.n      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/03/13 00:17:13 by Mika Schipp   #+#    #+#                 */
/*   Updated: 2025/03/13 01:37:08 by Mika Schipp   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "tokenize.h"

typedef struct t_tokeninfo
{
	e_token_type	lasttype;
	bool			cmdfound;
	size_t			index;
}					t_tokeninfo;

typedef struct t_token
{
	e_token_type	type;
	char			*value;
}	t_token;

t_env_var	**get_vars_from_cmd(char *cmd);
t_token		**get_tokens_from_cmd(char *cmd, t_env_var **vars);