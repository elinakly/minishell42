/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structbuild.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschippe <mschippe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 00:10:14 by Mika Schipp       #+#    #+#             */
/*   Updated: 2025/03/19 09:33:35 by mschippe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/structbuild.h"
#include "../../include/variable.h"
#include "../../include/tokenize.h"
#include "../../include/memory.h"

/**
 * TODO: Write docs
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
 * TODO: Write docs
 */
t_token	*make_token(char *raw_token, e_token_type type)
{
	t_token	*token;

	if (!raw_token)
		return (NULL);
	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->raw_value = raw_token;
	token->value = sanitize_token(raw_token);
	token->type = type;
	token->next = NULL;
	return (token);
}

/**
 * TODO: Write docs
 */
t_token	*get_tokens_from_cmd(char *cmd, t_env_var **vars, size_t *tokencount)
{
	t_tokeninfo		info;
	char			**tokens;
	t_token			*head;
	t_token			*next;
	t_token			**change;

	tokens = tokenize(cmd, vars, tokencount);
	if (!tokens)
		return (NULL);
	info = (t_tokeninfo){TT_UNKNOWN, false, 0};
	head = NULL;
	change = &head;
	while (tokens[info.index])
	{
		info.lasttype = get_token_type(tokens[info.index], info.lasttype,
				&info.cmdfound);
		*change = make_token(tokens[info.index++], info.lasttype);
		if (!*change)
			return (free_array((void **)tokens, NULL), NULL); // TODO: Free linkedlist @head too, needs func
		change = &((*change)->next);
	}
	free(tokens);
	return (head);
}

/**
 * TODO: Write docs
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
 * TODO: Write docs
 */
t_redirect	*create_redir(t_token *token)
{
	t_redirect	*res;

	if (!token || !token->next)
		return (NULL);
	res = malloc(sizeof(t_redirect));
	if (!res)
		return (NULL);
	res->type = get_redir_type(token);
	res->file = NULL;
	if (token->next->type == TT_OUTFILE || token->next->type == TT_INFILE)
		res->file = token->next->value;
	res->heredoc_delim = NULL;
	res->expand_in_heredoc = false;
	if (token->next->type == TT_HEREDOC_DELIM)
	{
		res->heredoc_delim = token->next->value;
		res->expand_in_heredoc = heredoc_should_expand(token->next->raw_value);
	}
	res->next = NULL;
	return (res);
}

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

bool	insert_into_command(t_command *cmd, t_token *token, size_t *argv_i)
{
	t_redirect	*tempre;
	
	if (token->type == TT_COMMAND)
	{
		cmd->name = token->value;
		cmd->has_command = true;
	}
	if (token->type == TT_ARGUMENT)
		cmd->argv[(*argv_i)++] = token->value;
	if (token->type == TT_HEREDOC || token->type == TT_RE_OUT_APPEND
		|| token->type == TT_RE_IN || token->type == TT_RE_OUT)
		return (insert_redir_in_cmd(cmd, token));
	return (true);
}

/**
 * TODO: Write docs
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
	res->argv = malloc(sizeof(char *) * (res->argc + 1));
	if (!res->argv)
		return (free(res), NULL); // TODO: Needs proper free, including stuff inside res
	res->argv[res->argc] = NULL;
	res->redirects = NULL;
	res->next = NULL;
	res->has_redirects = false;
	res->has_command = false;
	if (!insert_into_command(res, token, &argv_i))
		return (NULL); // TODO: Needs proper free, including stuff inside res
	return (res);
}

/**
 * TODO: write docs
 * TODO: ensure no token null check is ok (I NEED LINES)
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
				return (NULL); // TODO: Free everything inside cmd with some linkedlist free func
		else if (token->type == TT_PIPE)
		{
			token = token->next; //TODO: Technically could be NULL but that would have been syntax error? Double check or make note of this
			cmd->next = create_command(token);
			if (!cmd->next)
				return (NULL); // TODO: Free everything inside cmd with some linkedlist free func
			cmd = cmd->next;
			argv_i = 0;
		}
		token = token->next;
	}
	return (cmd_head);
}
