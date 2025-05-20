/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_info.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschippe <mschippe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 15:52:48 by eklymova          #+#    #+#             */
/*   Updated: 2025/05/20 18:15:57 by mschippe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Extracts all environment variables from a command string
 * @param cmd The command string to use
 * @returns An array of environment variable structures
 */
t_env_var	**get_vars_from_cmd(char *cmd, t_shell *shell, bool track_quotes)
{
	size_t		varcount;
	t_part_var	**varnames;
	t_env_var	**variables;

	if (!cmd)
		return (NULL);
	varcount = get_var_count(cmd, track_quotes);
	varnames = malloc(sizeof(t_part_var *) * (varcount + 1));
	if (!varnames)
		return (NULL);
	get_var_names(cmd, varcount, varnames, track_quotes);
	varnames[varcount] = NULL;
	variables = get_command_vars(varnames, shell);
	if (!variables)
		return (free_array((void **)variables, &clear_env_var),
			free_array((void **)varnames, &clear_part_var), NULL);
	free_array((void **)varnames, &clear_part_var);
	return (variables);
}

/**
 * Extracts all tokens from a command string
 * Requires an array of environment variables to expand before tokenizing
 * @param cmd The command string from which to extract
 * @param vars An array of environment variables
 * @param tokencount A pointer to a size_t that will be
 * set to the amount of tokens that were extracted (Can NOT be NULL)
 * @returns A linkedlist with extracted token structs
 */
t_token	*get_tokens_from_cmd(char *cmd, t_env_var **vars, size_t *tokencount)
{
	t_tokeninfo		info;
	char			**tokens;
	t_token			*top;
	t_token			*next;
	t_token			**change;

	if (!tokencount)
		return (NULL);
	tokens = tokenize(cmd, vars, tokencount);
	if (!tokens)
		return (NULL);
	info = (t_tokeninfo){TT_UNKNOWN, false, 0};
	top = NULL;
	change = &top;
	while (tokens[info.index])
	{
		info.lasttype = get_token_type(tokens[info.index], info.lasttype,
				&info.cmdfound);
		*change = make_token(tokens[info.index++], info.lasttype);
		if (!*change)
			return (free_array((void **)tokens, NULL), free_tokens(top), NULL);
		change = &((*change)->next);
	}
	free_array((void **)tokens, NULL);
	return (top);
}

/**
 * Converts token type into redirect type based on a given token
 * Defaults to RE_UNKNOWN if original type is not some sort of redirect
 * @param token The token whose type to convert
 * @returns A redirect type
 */
t_redir_type	get_redir_type(t_token *token)
{
	if (!token)
		return (RE_UNKNOWN);
	if (token->type == TT_RE_IN)
		return (RE_INPUT);
	if (token->type == TT_RE_OUT)
		return (RE_OUTPUT_TRUNC);
	if (token->type == TT_RE_OUT_APPEND)
		return (RE_OUTPUT_APPEND);
	if (token->type == TT_HEREDOC)
		return (RE_HEREDOC);
	return (RE_UNKNOWN);
}

/**
 * Loops through token linkedlist to find any argument types up until a pipe
 * @param token The token from which to start counting arguments
 * @returns The amount of arguments in a command
 */
size_t	get_cmd_argc(t_token *token)
{
	size_t	argc;

	argc = 0;
	if (!token)
		return (argc);
	while (token && token->type != TT_PIPE)
	{
		argc += token->type == TT_ARGUMENT;
		token = token->next;
	}
	return (argc);
}
