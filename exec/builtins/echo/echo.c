#include <unistd.h>

void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}

void	ft_putstr_fd(char *s, int fd)
{
	int	i;

	i = 0;
	while (s[i] != '\0')
	{
		write(fd, &s[i], 1);
		i++;
	}
}

int	check_flag(char **args)
{
	int	flag;
	int	i;
	int	j;

	i = 1;
	flag = 0;
	while (args[i])
	{
		j = 0;
		if (args[i][j] == '-' && args[i][j + 1] == 'n')
		{
			j++;
			while (args[i][j] == 'n')
				j++;
			if (!args[i][j])
				flag = 1;
			else
				break ;
		}
		else
			break ;
		i++;
	}
	return (flag);

}

void	exo(char **args)
{
	int	i;
	int	flag;

	if (!args[1])
		ft_putchar_fd('\n', 1);
	flag = check_flag(args);
	if (flag == 1)
		i = 2;
	else
		i = 1;
	while (args[i])
	{
		ft_putstr_fd(args[i], 1);
		if (args[i + 1])
			ft_putchar_fd(' ', 1);
		i++;
	}
	if (flag == 0)
		ft_putchar_fd('\n', 1);
}

int	main(int argc, char **argv)
{
	exo(argv);
	return(0);
}
