#include <unistd.h>

int	ft_putchar_fd(char c, int fd)
{
	int	status;

	status = write(fd, &c, 1);
	return (status == -1);
}

int	ft_putstr_fd(char *s, int fd)
{
	int	i;
	int	status;

	i = 0;
	while (s[i] != '\0')
	{
		status = write(fd, &s[i], 1);
		if (status  == -1)
			return 1;
		i++;
	}
	return (0);
}

int	check_flag(char **args)
{
	int	flag;
	int	j;

	*i = 1;
	flag = 0;
	while (args[*i])
	{
		j = 0;
		if (args[*i][j] == '-' && args[*i][j + 1] == 'n')
		{
			j++;
			while (args[*i][j] == 'n')
				j++;
			if (!args[*i][j])
				flag = 1;
			else
				break ;
		}
		else
			break ;
		(*i)++;
	}
	return (flag);
}

int	exo(char **args)
{
	int	i;
	int	flag;
	int	status;

	status = 0;
	if (!args[1])
		return (ft_putchar_fd('\n', 1));
	flag = check_flag(args, &i);
	while (args[i])
	{
		status = ft_putstr_fd(args[i], 1);
		if (args[i + 1])
			status = ft_putchar_fd(' ', 1);
		if (status != 0)
			return (status);
		i++;
	}
	if (flag == 0)
		return (ft_putchar_fd('\n', 1));
	return (0);
}

int	main(int argc, char **argv)
{
	return(exo(argv));
}
