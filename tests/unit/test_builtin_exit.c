#include "minishell.h"

#define ERROR_MISUSAGE 2

int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && *s2)
	{
		if (*s1 != *s2)
			return ((unsigned char)*s1 - (unsigned char)*s2);
		s1++;
		s2++;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}

/* static bool	is_valid_number(char *str)
{
	int	i;

	if (!str || str[0] == '\0')
		return (false);
	i = 0;
	// exit allows one '-' or '+'
	if (str[i] == '-' || str[i] == '+')
		i = 1;
	// reject solo '-' or solo '+'
	if (str[i] == '\0')
		return (false);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (false);
		i++;
	}
	return (true);
} */

int	builtin_exit(char **tokens, t_shell *data)
{
	long	exit_code;

	// exit without argument, returns the last known exit code
	if (tokens[1] == NULL)
		return (data->last_exit_code);

	// exit with valid number as argument, return code (mod 256 will convert code to be in range [0-255] if needed)
	if (tokens[2] == NULL)
	{
		if (ft_safe_atol(tokens[1], &exit_code) == 1)
				return ((int)(exit_code % 256));
		else
		{
			printf("mini$Hell: exit: %s: numeric argument required\n", tokens[1]);
			return (ERROR_MISUSAGE);
		}
	}
	// exit whith too many arguments won't exit the shell, but return 1
	printf("mini$Hell: exit: too many arguments\n");
	return (EXIT_FAILURE);
}


void	test_exit_no_args(void)
{
	printf(BR_BLU "TEST 1: exit without arguments\n" RESET);

	char	*tokens[] =
	{
		"exit",
		NULL
	};
	t_shell	data =
	{
		.env_list = NULL,
		.last_exit_code = 42, // simulating random previous value stored
		.is_interactive_mode = true,
		.is_child = false
	};
	int	result = builtin_exit(tokens, &data);
	printf("context: exit without argument will return the last known exit code stored\n");
	printf(YEL "expected:\t42 - shell should terminate, not tested in this unit test, see (non existent yet) integration test\n" RESET);
	printf(YEL "result:\t\t%d\n" RESET, result);
	printf("RESULT: %s\n\n", result == 42 ? GRN "PASS" RESET : RED "FAIL" RESET);
}

void	test_exit_arg_number(void)
{
	 printf(BR_BLU "TEST 2: exit with arg (valid number)\n" RESET);

	char	*tokens[] =
	{
		"exit",
		"123",
		NULL
	};
	t_shell	data =
	{
		.env_list = NULL,
		.last_exit_code = 42, // simulating random previous value stored
		.is_interactive_mode = true,
		.is_child = false
	};
	int result = builtin_exit(tokens, &data);
	printf("context: exit with valid argument will return the argument passed or will modulo 256 it if arg is out of [0-255] range\n");
	printf(YEL "expected:\t123 - shell should terminate, not tested in this unit test, see (non existent yet) integration test\n" RESET);
	printf(YEL "result:\t\t%d\n" RESET, result);
	printf("RESULT: %s\n\n", result == 123 ? GRN "PASS" RESET : RED "FAIL" RESET);
}

void	test_exit_too_many_args(void)
{
	 printf(BR_BLU "TEST 3: exit with too many args (invalid)\n" RESET);

	char	*tokens[] =
	{
		"exit",
		"123",
		"456",
		"hello",
		NULL
	};
	t_shell	data =
	{
		.env_list = NULL,
		.last_exit_code = 42, // simulating random previous value stored
		.is_interactive_mode = true,
		.is_child = false
	};

	printf(YEL "expected:\tmessage + code 1 + shell stays open\n" RESET);
	int result = builtin_exit(tokens, &data);
	printf(YEL "result:\t\tcode %d\n" RESET, result);
	printf("RESULT: %s\n\n", result == 1 ? GRN "PASS" RESET : RED "FAIL" RESET);
}

int	main(void)
{
	printf(MAG "=== EXIT BUILTIN TESTS ===" RESET "\n\n");

	test_exit_no_args();
	test_exit_arg_number();
	test_exit_too_many_args();
	return (0);
}
