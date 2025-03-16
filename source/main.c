/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: eklymova <eklymova@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/03/06 15:24:30 by eklymova      #+#    #+#                 */
/*   Updated: 2025/03/16 17:27:33 by Mika Schipp   ########   odam.nl         */
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
		default: "UNKNOWN PARSE RESULT?!";
	}
}

void test_parse_output(char *test, bool isdebug)
{
	e_parse_result strparseres = validate_cmd_str(test);
	if (isdebug && strparseres != PARSEOK)
		printf("Parsing validation: %s\n", parse_result_to_string(strparseres));
	if (strparseres != PARSEOK)
		return;
	t_env_var **variables = get_vars_from_cmd(test);
	if (!variables)
		return;
	t_token **tokens = get_tokens_from_cmd(test, variables);
	free_array((void **)variables, &clear_env_var);
	if (!tokens)
		return;
	size_t tokenindex = 0;
	if (isdebug)
	{
		while (tokens[tokenindex])
		{
			printf("%-16s [%s]\n", token_type_to_string(tokens[tokenindex]->type), 
				tokens[tokenindex]->value);
			tokenindex++;
		}
		e_parse_result parseres = validate_tokens(tokens);
		printf("Parsing validation: %s\n", parse_result_to_string(parseres));
	}
	free_array((void **)tokens, &clear_token_var);
}
void test_execute(char *test, char **envp)
{
	char **args = ft_split(test, ' ');
	int builtin = is_builtin(test, args, envp);
}

int	main(int argc, char **argv, char **envp)
{
	char *test;
	set_signal();	
	while (1)
	{
		if (!(test = ft_readline(envp)))
			return (1);
		test_execute(test, envp);
		// Just made this function to keep main() a bit readable
		test_parse_output(test, argc == 2 && !ft_strncmp(argv[1], "--debug", 255));
		add_history(test);
		free(test);
	}
	rl_clear_history();
	return (0);
}