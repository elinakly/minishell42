/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_tokens_var.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eklymova <eklymova@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 15:52:39 by eklymova          #+#    #+#             */
/*   Updated: 2025/05/20 16:04:37 by eklymova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include "../../include/tokenize.h"
#include "../../include/structbuild.h"
#include "../../include/path.h"

void	free_venv(t_venv *base)
{
	t_venv	*next;

	if (!base)
		return ;
	while (base)
	{
		next = base->next;
		if (!base->base)
		{
			free(base->name);
			free(base->value);
		}
		free(base);
		base = next;
	}
}

/**
 * Frees a single token and all its allocated fields
 * @param token The token to free
 */
void	free_single_token(t_token *token)
{
	if (!token)
		return ;
	free(token->raw_value);
	free(token->value);
	free(token);
}

/**
 * Loops through token linkedlist and frees all of them
 * @param head The first token in the linkedlist
 */
void	free_tokens(t_token *head)
{
	t_token	*next;

	while (head)
	{
		next = head->next;
		free_single_token(head);
		head = next;
	}
}

/**
 * Frees the data inside of a partial environment variable
 * @param var The partial variable to free data in (cast to void pointer)
 */
void	clear_part_var(void *var)
{
	t_part_var	*realvar;

	realvar = (t_part_var *)var;
	if (var)
		free(realvar->name);
}

/**
 * Deletes the name and value strings inside of an environment variable struct
 * @param var The variable struct (cast to a void pointer) to target
 */
void	clear_env_var(void *var)
{
	t_env_var	*realvar;

	realvar = (t_env_var *)var;
	if (var)
	{
		free(realvar->name);
		free(realvar->value);
	}
}
