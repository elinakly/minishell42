/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschippe <mschippe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 15:24:30 by eklymova          #+#    #+#             */
/*   Updated: 2025/03/06 19:37:05 by mschippe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "../include/tokenize.h"
#include "../include/memory.h"
#include <stdbool.h>
#include <stdio.h>

char			*ft_readline(char **envp);
int				skip_spaces(char **str);
int				skip_quoted(char **str);
bool			has_unclosed_quote(char *str, e_quote_type *type);
size_t			count_tokens(char *entry);
char			**tokenize(char *entry, size_t *tokencount);
e_token_type	get_token_type(char *raw_token, e_token_type last, bool *cmdfound);
size_t			get_var_count(char *cmd);

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

int	main(int argc, char **argv, char **envp)
{
	int tokenindex = 0;
	size_t amount = 0;
	e_token_type lasttype = TT_UNKNOWN;
	bool cmdfound = false;
	
	while (1)
	{
		char *test = ft_readline(envp);
		char **tokens = tokenize(test, &amount);
		tokenindex = 0;
		printf("%s\n[has %ld vars]\n", test, get_var_count(test));
		while (tokens[tokenindex])
		{
			e_token_type tokentype = get_token_type(tokens[tokenindex], lasttype, &cmdfound);
			lasttype = tokentype;
			printf("%-16s [%s]\n", token_type_to_string(tokentype), tokens[tokenindex]);
			tokenindex++;
		}
		lasttype = TT_UNKNOWN;
		free_array((void **)tokens);
		cmdfound = false;
	}
	return (0);
}