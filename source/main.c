/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: eklymova <eklymova@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/03/06 15:24:30 by eklymova      #+#    #+#                 */
/*   Updated: 2025/03/09 16:48:32 by Mika Schipp   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdbool.h>
#include "../include/tokenize.h"
#include "../include/memory.h"
#include "../include/builtins.h"
#include "../include/variable.h"

char			*ft_readline(char **envp);
int				skip_spaces(char **str);
int				skip_quoted(char **str);
bool			has_unclosed_quote(char *str, e_quote_type *type);
size_t			count_tokens(char *entry);
char			**tokenize(char *entry, size_t *tokencount);
e_token_type	get_token_type(char *raw_token, e_token_type last, bool *cmdfound);
size_t			get_var_count(char *cmd);
char			**get_var_names(char *cmd);
int				is_builtin(char *command);
t_env_var		**get_command_vars(char **envp, char **names);
size_t			calc_expanded_len(char *cmd, t_env_var **vars);

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

void test_parse_output(char *test, char** envp)
{
	int tokenindex = 0;
	size_t amount = 0;
	size_t varindex = 0;
	e_token_type lasttype = TT_UNKNOWN;
	bool cmdfound = false;

	char **tokens = tokenize(test, &amount);
	tokenindex = 0;
	char **varnames = get_var_names(test);
	t_env_var **variables = get_command_vars(envp, varnames);
	while (variables[varindex])
	{
		printf("[VAR: %s -> \"%s\"]\n", variables[varindex]->name,
				variables[varindex]->value);
		varindex++;
	}
	printf("Expanded length would be: %ld\n", calc_expanded_len(test, variables));
	while (tokens[tokenindex])
	{
		e_token_type tokentype = get_token_type(tokens[tokenindex], lasttype, &cmdfound);
		lasttype = tokentype;
		printf("%-16s [%s]\n", token_type_to_string(tokentype), tokens[tokenindex]);
		tokenindex++;
	}
	free_array((void **)tokens);
	free_array((void **)varnames);
}

int	main(int argc, char **argv, char **envp)
{
	char *test;
	while (1)
	{
		if (!(test = ft_readline(envp)))
			return (1);
		// int builtin = is_builtin(test);
		// Just made this function to keep main() a bit readable
		test_parse_output(test, envp);
		free(test);
	}
	return (0);
}