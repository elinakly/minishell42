/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   insert.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eklymova <eklymova@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 15:52:52 by eklymova          #+#    #+#             */
/*   Updated: 2025/05/20 16:04:06 by eklymova         ###   ########.fr       */
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
 * Takes a command struct and inserts a redirect struct into it
 * @param cmd The command in which to insert a redirect
 * @param token The token from which to create a redirect
 * (MUST be of a redirect type)
 * @returns `true` if all allocations in the process succeeded, `false` if not
 */
bool	insert_redir_in_cmd(t_command *cmd, t_token *token)
{
	t_redirect	*tempre;
	t_redirect	*head;

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
 * TODO: Find a way to insert path into argv[0] always
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
		cmd->has_command = true;
		if (!cmd->name)
			return (false);
		free(cmd->argv[0]);
		cmd->argv[0] = ft_strdup(token->value);
		if (!cmd->argv[0])
			return (false);
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
