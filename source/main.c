/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: eklymova <eklymova@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/03/06 15:24:30 by eklymova      #+#    #+#                 */
/*   Updated: 2025/04/25 18:59:08 by Mika Schipp   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdbool.h>
#include "../include/tokenize.h"
#include "../include/variable.h"
#include "../include/path.h"
#include "../include/memory.h"
#include "../include/builtins.h"
#include "../include/execute.h"
#include "../include/structbuild.h"
#include "../include/validate.h"
#include "../include/minishell.h"
#include "../include/venv.h"
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
	t_command		*cmds;
	t_shell			shell;

	set_signal();
	if (get_history())
		return (1);
	//TODO: Check cwd and venv NULL in shell
	//TODO: Free everything inside shell, some even still need functions made
	shell = (t_shell){NULL, NONE, 0, get_cwd(), make_venv(envp), true};
	while (shell.loop_active)
	{
		shell.main_rl_str = ft_readline(envp);
		if (ft_strncmp(shell.main_rl_str, "debug", 6) == 0)
		{
			shell.loop_active = false;
			break;
		}
		if (!shell.main_rl_str)
			return (1);
		shell.last_parse_res = parse_commands(shell, &cmds);
		if (shell.last_parse_res == PARSEOK)
		{
			shell.last_status = execute_cmds(cmds, envp, ft_cmdcount(cmds));
			free_commands(cmds);
		}
		add_history(shell.main_rl_str);
		history(shell.main_rl_str);
		free(shell.main_rl_str);
	}
	// TODO: We can't exit the loop so this is probably never actually reached, we will need to handle it in our exit functions
	rl_clear_history();
	free_path(shell.cwd, true);
	free_venv(shell.venv);
	return (0); // probably? maybe needs to be different depending on signal or whatever
}
