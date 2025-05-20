/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structbuild.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eklymova <eklymova@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 00:10:14 by Mika Schipp       #+#    #+#             */
/*   Updated: 2025/05/20 16:21:23 by eklymova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/structbuild.h"
#include "../../include/variable.h"
#include "../../include/tokenize.h"
#include "../../include/memory.h"
#include "../../include/validate.h"
#include "../../include/venv.h"
#include "../../include/minishell.h"
#include "../../lib/libft/libft.h"

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
 * Allocates for and creates a single redirect struct
 * Fills in relevant details about the redirect based on tokens that are ahead
 * TODO: Bit dodgy, strdup will happen and we assume it'll be inserted SOMEWHERE
 * always, and thus freed somewhere always,
 * it SHOULD be true but gotta triple check
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

	argv_i = 1;
	res = malloc(sizeof(t_command));
	if (!res)
		return (NULL);
	res->name = NULL;
	res->argc = get_cmd_argc(token) + 1;
	res->argv = NULL;
	res->argv = malloc(sizeof(char *) * (res->argc + 1));
	if (!res->argv)
		return (free_single_cmd(res), NULL);
	res->argv[res->argc] = NULL;
	res->argv[0] = ft_strdup(""); // TODO: Must become path
	//somewhere, no NULL default because that could break array free func
	if (!res->argv[0])
		return (free_single_cmd(res), NULL);
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

	argv_i = 1;
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
			argv_i = 1;
		}
		token = token->next;
	}
	return (cmd_head);
}

/**
 * Parses a command string into a useable linked list containing commands
 * @param cmdstr The command string to parse
 * @param cmd A pointer to where the linked list should be stored
 * @returns A parsing result that describes what happened in parsing
 */
e_parse_result	parse_commands(t_shell *shell, t_command **cmd)
{
	size_t			tokencount;
	e_parse_result	res;
	t_env_var		**variables;
	t_token			*tokens;

	tokencount = 0;
	res = validate_cmd_str(shell->main_rl_str);
	if (res != PARSEOK)
		return (res);
	variables = get_vars_from_cmd(shell->main_rl_str, shell, true);
	if (!variables)
		return (MALLOC_FAIL);
	tokens = get_tokens_from_cmd(shell->main_rl_str, variables, &tokencount);
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
