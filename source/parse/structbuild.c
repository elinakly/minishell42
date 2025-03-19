/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structbuild.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschippe <mschippe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 00:10:14 by Mika Schipp       #+#    #+#             */
/*   Updated: 2025/03/19 13:02:09 by mschippe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/structbuild.h"
#include "../../include/variable.h"
#include "../../include/tokenize.h"
#include "../../include/memory.h"
#include "../../include/validate.h"
#include "../../lib/libft/libft.h"

/**
 * Extracts all environment variables from a command string
 * @param cmd The command string to use
 * @returns An array of environment variable structures
 */
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

/**
 * Allocates memory for and creates a single token
 * Assigns processed and raw values to it, both of which are new allocations
 * that must be freed.
 * @param raw_token The raw, unprocessed token value
 * @param type The corresponding token type
 * @returns A single new token structure
 */
t_token	*make_token(char *raw_token, e_token_type type)
{
	t_token	*token;

	if (!raw_token)
		return (NULL);
	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->raw_value = ft_strdup(raw_token);
	if (!token->raw_value)
		return (free_single_token(token), NULL);
	token->value = sanitize_token(raw_token);
	if (!token->value)
		return (free_single_token(token), NULL);
	token->type = type;
	token->next = NULL;
	return (token);
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
e_redir_type	get_redir_type(t_token *token)
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
 * Allocates for and creates a single redirect struct
 * Fills in relevant details about the redirect based on tokens that are ahead
 * TODO: Bit dodgy, strdup will happen and we assume it'll be inserted SOMEWHERE always, and thus freed somewhere always, it SHOULD be true but gotta triple check
 * @param token A token that is of some redirect type.
 * Can not be the last token in the list as it requires a file or delimiter.
 * @returns a single redirect struct
 */
t_redirect	*create_redir(t_token *token)
{
	t_redirect	*res;
	char		*valuecpy;

	if (!token || !token->next)
		return (NULL);
	res = malloc(sizeof(t_redirect));
	if (!res)
		return (NULL);
	res->file = NULL;
	res->next = NULL;
	res->heredoc_delim = NULL;
	res->expand_in_heredoc = false;
	res->type = get_redir_type(token);
	valuecpy = ft_strdup(token->next->value);
	if (!valuecpy)
		return (free_single_redir(res), NULL);
	if (token->next->type == TT_OUTFILE || token->next->type == TT_INFILE)
		res->file = valuecpy;
	else if (token->next->type == TT_HEREDOC_DELIM)
	{
		res->heredoc_delim = valuecpy;
		res->expand_in_heredoc = heredoc_should_expand(token->next->raw_value);
	}
	return (res);
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

/**
 * Takes a command struct and inserts a redirect struct into it
 * @param cmd The command in which to insert a redirect
 * @param token The token from which to create a redirect
 * (MUST be of a redirect type)
 * @returns `true` if all allocations in the process succeeded, `false` if not
 */
bool	insert_redir_in_cmd(t_command *cmd, t_token *token)
{
	t_redirect	*tempre;
	t_redirect *head;

	tempre = create_redir(token);
	if (!tempre)
		return (false);
	cmd->has_redirects = true;
	if (!cmd->redirects)
		cmd->redirects = tempre;
	else
	{
		head = cmd->redirects;
		while (head->next)
			head = head->next;
		head->next = tempre;
	}
	return (true);
}

/**
 * Inserts token data into a command struct
 * @param cmd The command in which to insert data
 * @param token The token from which to take data
 * @param argv_i `size_t` pointer to keep track of argument index
 * @returns `true` if all allocations in the process succeeded, `false` if not
 */
bool	insert_into_command(t_command *cmd, t_token *token, size_t *argv_i)
{
	t_redirect	*tempre;
	
	if (token->type == TT_COMMAND && !cmd->has_command)
	{
		cmd->name = ft_strdup(token->value);
		if (!cmd->name)
			return (false);
		cmd->has_command = true;
	}
	else if (token->type == TT_ARGUMENT)
	{
		cmd->argv[*argv_i] = ft_strdup(token->value);
		if (!cmd->argv[*argv_i])
			return (false);
		(*argv_i)++;
	}
	else if (token->type == TT_HEREDOC || token->type == TT_RE_OUT_APPEND
		|| token->type == TT_RE_IN || token->type == TT_RE_OUT)
		return (insert_redir_in_cmd(cmd, token));
	return (true);
}

/**
 * Allocates for and creates a single command struct.
 * Also inserts some relevant data into the command based on the provided token
 * @param token The first token in the command.
 * Does not have to be of command type
 * @returns A single command struct
 */
t_command	*create_command(t_token *token)
{
	t_command	*res;
	size_t		argv_i;

	argv_i = 0;
	res = malloc(sizeof(t_command));
	if (!res)
		return (NULL);
	res->name = NULL;
	res->argc = get_cmd_argc(token);
	res->argv = NULL;
	res->argv = malloc(sizeof(char *) * (res->argc + 1));
	if (!res->argv)
		return (free_single_cmd(res), NULL);
	res->argv[res->argc] = NULL;
	res->redirects = NULL;
	res->next = NULL;
	res->has_redirects = false;
	res->has_command = false;
	if (!insert_into_command(res, token, &argv_i))
		return (free_single_cmd(res), NULL);
	return (res);
}

/**
 * Creates a list of command structs based on a given list of tokens
 * @param token The first token in the list of tokens to use
 * @returns a linked list of command structs
 */
t_command	*make_cmd_list(t_token *token)
{
	t_command	*cmd;
	t_command	*cmd_head;
	size_t		argv_i;

	argv_i = 0;
	cmd = create_command(token);
	cmd_head = cmd;
	if (!cmd)
		return (NULL);
	while (token)
	{
		if (token->type != TT_PIPE && !insert_into_command(cmd, token, &argv_i))
				return (free_commands(cmd_head), NULL);
		else if (token->type == TT_PIPE)
		{
			token = token->next;
			cmd->next = create_command(token);
			if (!cmd->next)
				return (free_commands(cmd_head), NULL);
			cmd = cmd->next;
			argv_i = 0;
		}
		token = token->next;
	}
	return (cmd_head);
}

e_parse_result	parse_commands(char *cmdstr, t_command **cmd)
{
	size_t			tokencount;
	e_parse_result	res;
	t_env_var		**variables;
	t_token			*tokens;

	tokencount = 0;
	res = validate_cmd_str(cmdstr);
	if (res != PARSEOK)
		return (res);
	variables = get_vars_from_cmd(cmdstr);
	if (!variables)
		return (MALLOC_FAIL);
	tokens = get_tokens_from_cmd(cmdstr, variables, &tokencount);
	free_array((void **)variables, &clear_env_var);
	if (!tokens)
		return (MALLOC_FAIL);
	res = validate_tokens(tokens);
	if (res != PARSEOK)
		return (res);
	*cmd = make_cmd_list(tokens);
	if (!*cmd)
		return (MALLOC_FAIL);
	free_tokens(tokens);
	return (PARSEOK);
}
