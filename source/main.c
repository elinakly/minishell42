/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschippe <mschippe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 15:24:30 by eklymova          #+#    #+#             */
/*   Updated: 2025/03/19 13:16:35 by mschippe         ###   ########.fr       */
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

const char *token_type_to_string(e_token_type type)
{
	switch (type)
	{
		case TT_UNKNOWN: return "TT_UNKNOWN";
		case TT_COMMAND: return "TT_COMMAND";
		case TT_ARGUMENT: return "TT_ARGUMENT";
		case TT_HEREDOC: return "TT_HEREDOC";
		case TT_PIPE: return "TT_PIPE";
		case TT_RE_IN: return "TT_RE_IN";
		case TT_RE_OUT: return "TT_RE_OUT";
		case TT_RE_OUT_APPEND: return "TT_RE_OUT_APPEND";
		case TT_INFILE: return "TT_INFILE";
		case TT_OUTFILE: return "TT_OUTFILE";
		case TT_HEREDOC_DELIM: return "TT_HEREDOC_DELIM";
		default: return "INVALID_TYPE";
	}
}

const char *parse_result_to_string(e_parse_result res)
{
	switch (res)
	{
		case UNCLOSED_SQUOTE: return "UNCLUSED_SQUOTE";
		case UNCLOSED_DQUOTE: return "UNCLOSED_DQUOTE";
		case UNFINISHED_PIPE: return "UNFINISHED_PIPE";
		case ESCAPED_NEWLINE: return "ESCAPED_NEWLINE";
		case HEREDOC:	return "HEREDOC";
		case SYNTAX_ERROR: return "SYNTAX_ERROR";
		case MALLOC_FAIL: return "MALLOC_FAIL";
		case EMPTY: return "EMPTY";
		case PARSEOK: return "PARSEOK";
	}
}

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
			if (argc == 2 && !ft_strncmp(argv[1], "--debug", 255))
				print_command_list(cmds);
			free_commands(cmds);
		}
		add_history(cmdstr);
		free(cmdstr);
	}
	rl_clear_history(); // TODO: We can't exit the loop so this is probably never actually reached, we will need to handle it in our exit functions
	return (0);
}
