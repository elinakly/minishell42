/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschippe <mschippe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 15:24:30 by eklymova          #+#    #+#             */
/*   Updated: 2025/03/19 14:19:35 by mschippe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdbool.h>
#include "../include/tokenize.h"
#include "../include/variable.h"
#include "../include/memory.h"
#include "../include/builtins.h"
#include "../include/execute.h"
#include "../include/structbuild.h"
#include "../include/validate.h"
#include <readline/history.h>

char			*ft_readline(char **envp);

void print_redirects(t_redirect *head)
{
	t_redirect	*current;

	current = head;
	while (current)
	{
		printf("  - [Type: %d | File: \"%s\" | Delimiter: \"%s\" | Heredoc expand: %s]\n",
			current->type, current->file ? current->file : "(none)", 
			current->heredoc_delim ? current->heredoc_delim : "(none)",
			current->expand_in_heredoc ? "Yes" : "No");
		current = current->next;
	}
}

void print_command_list(t_command *head) // Thank you kindly, ChatGPT
{
	t_command	*current;
	size_t		i;

	current = head;
	while (current)
	{
		printf("Command: %s\n", current->name ? current->name : "(none)");
		i = 0;
		if (current->argc > 0)
		{
			printf("Arguments:\n");
			while (i < current->argc)
			{
				printf("  - [%s]\n", current->argv[i]);
				i++;
			}
		}
		if (current->has_redirects)
		{
			printf("Redirects:\n");
			print_redirects(current->redirects);
		}
		printf("------------\n");
		current = current->next;
	}
}

void new_test_exec(t_command *cmds)
{
	while (cmds)
	{
		if (cmds->has_command && ft_strncmp(cmds->name, "echo", 4) == 0)
			echo(cmds->argv);
		cmds = cmds->next;
	}
}

void test_execute(char *test, char **envp)
{
	char **args = ft_split(test, ' '); // TODO: Currently leaks, but will be replaced with parsed stuff anyway
	int builtin = is_builtin(test, args, envp);
}

int	main(int argc, char **argv, char **envp)
{
	char			*cmdstr;
	e_parse_result	result;
	t_command		*cmds;

	set_signal();
	while (1)
	{
		cmdstr = ft_readline(envp);
		if (!cmdstr)
			return (1);
		//test_execute(cmdstr, envp);
		result = parse_commands(cmdstr, &cmds);
		if (result == PARSEOK)
		{
			new_test_exec(cmds);
			free_commands(cmds);
		}
		add_history(cmdstr);
		free(cmdstr);
	}
	rl_clear_history(); // TODO: We can't exit the loop so this is probably never actually reached, we will need to handle it in our exit functions
	return (0);
}
