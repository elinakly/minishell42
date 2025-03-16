/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   structbuild.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: Mika Schipper <mschippe@student.codam.n      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/03/13 00:10:14 by Mika Schipp   #+#    #+#                 */
/*   Updated: 2025/03/16 17:03:21 by Mika Schipp   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/structbuild.h"
#include "../../include/variable.h"
#include "../../include/tokenize.h"
#include "../../include/memory.h"

t_env_var	**get_vars_from_cmd(char *cmd)
{
	size_t		varcount;
	t_part_var	**varnames;
	t_env_var	**variables;

	if (!cmd)
		return (NULL);
	varcount = get_var_count(cmd);
	varnames = malloc(sizeof(t_part_var *) * (varcount + 1));
	if (!varnames)
		return (NULL);
	get_var_names(cmd, varcount, varnames);
	varnames[varcount] = NULL;
	variables = get_command_vars(varnames);
	if (!variables)
		return(free_array((void **)variables, &clear_env_var),
		free_array((void **)varnames, &clear_part_var), NULL);
	free_array((void **)varnames, &clear_part_var);
	return (variables);
}

t_token	*make_token(char *raw_token, e_token_type type)
{
	t_token	*token;

	if (!raw_token)
		return (NULL);
	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = sanitize_token(raw_token);
	token->type = type;
	return (token);
}

t_token	**get_tokens_from_cmd(char *cmd, t_env_var **vars)
{
	t_tokeninfo		info;
	char			**tokens;
	t_token			**res;
	size_t			amount;

	tokens = tokenize(cmd, vars, &amount);
	if (!tokens)
		return (NULL);
	res = malloc (sizeof(t_token *) * (amount + 1));
	if (!res)
		return (free_array((void **)tokens, NULL), NULL);
	res[amount] = NULL;
	info = (t_tokeninfo){TT_UNKNOWN, false, 0};
	while (tokens[info.index])
	{
		info.lasttype = get_token_type(tokens[info.index], info.lasttype,
				&info.cmdfound);
		res[info.index] = make_token(tokens[info.index], info.lasttype);
		if (!res[info.index++])
			return (free_array((void **)tokens, NULL),
					free_array((void **)res, NULL), NULL); //TODO: Del function for t_token **
	}
	free(tokens);
	return (res);
}

t_redirect	*create_redir(e_redir_type type, char *file)
{
	t_redirect	*res;

	res = malloc(sizeof(t_redirect));
	if (!res)
		return (NULL);
	res->type = type;
	res->file = file;
	res->next = NULL;
	return (res);
}

t_command	*create_command(char *name)
{
	t_command	*res;

	res = malloc(sizeof(t_command));
	if (!res)
		return (NULL);
	res->name = name;
	res->argc = 0;
	res->argv = NULL;
	res->has_pipe = false;
	res->has_redirects = false;
	res->redirects = NULL;
	res->next = NULL;
	return (res);
}
