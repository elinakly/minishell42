#include <stdio.h>
#include "../include/builtins.h"

int	env(char **envp)
{
	int	i;

	while(*envp)
	{
		i = 0;
		while(envp[0][i])
		{
			printf("%c", envp[0][i]);
			i++;
		}
		printf("\n");
		envp++;
	}
	return (0);
}

// int	main(int argc, char **argv, char **envp)
// {
// 	(void)argc;
// 	(void)argv;
// 	env(envp);
// 	return (0);
// }