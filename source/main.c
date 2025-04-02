/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: eklymova <eklymova@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/03/06 15:24:30 by eklymova      #+#    #+#                 */
/*   Updated: 2025/04/03 00:58:19 by Mika Schipp   ########   odam.nl         */
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
size_t			ft_cmdcount(t_command *head);

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

/**
 * TODO: Delete again, ChatGPT generated crap just for testing
 */
void	print_parse_result(e_parse_result e)
{
	const char *names[] = {
		"UNCLOSED_SQUOTE",
		"UNCLOSED_DQUOTE",
		"UNFINISHED_PIPE",
		"ESCAPED_NEWLINE",
		"HEREDOC",
		"SYNTAX_ERROR",
		"MALLOC_FAIL",
		"EMPTY",
		"PARSEOK"
	};

	if (e >= 0 && e <= PARSEOK)
		printf("%s\n", names[e]);
	else
		printf("UNKNOWN ENUM VALUE\n");
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


int	main(int argc, char **argv, char **envp)
{
	char			*cmdstr;
	e_parse_result	result;
	t_command		*cmds;

	set_signal();

	if (get_history())
		return (1);
	while (1)
	{
		cmdstr = ft_readline(envp);

		if (!cmdstr)
			return (1);
		result = parse_commands(cmdstr, &cmds);
		if (result == PARSEOK)
		{
			print_command_list(cmds);
			execute_cmds(cmds, envp, ft_cmdcount(cmds));
			free_commands(cmds);
		}
		else
			print_parse_result(result);
		add_history(cmdstr);
		history(cmdstr);
		free(cmdstr);
	}
	rl_clear_history(); // TODO: We can't exit the loop so this is probably never actually reached, we will need to handle it in our exit functions
	return (0);
}
