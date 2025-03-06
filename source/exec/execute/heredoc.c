#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h> 
#include "../../../lib/libft/libft.h" // Hi Elina I added these "by accident" maybe u wanted them in a header file instead  */

// void	here_doc(char *limiter, int argc)
// {
// 	pid_t	reader;
// 	int		fd[2];
// 	char	*line;

// 	if (argc < 6)
// 		usage();
// 	if (pipe(fd) == -1)
// 		error();
// 	reader = fork();
// 	if (reader == 0)
// 	{
// 		close(fd[0]);
// 		while (get_next_line(&line))
// 		{
// 			if (ft_strncmp(line, limiter, ft_strlen(limiter)) == 0)
// 				exit(EXIT_SUCCESS);
// 			write(fd[1], line, ft_strlen(line));
// 		}
// 	}
// 	else
// 	{
// 		close(fd[1]);
// 		dup2(fd[0], STDIN_FILENO);
// 		wait(NULL);
// 	}
// } // Hi I've commented it because it break Makefile 