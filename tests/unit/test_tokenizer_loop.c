#include "minishell.h"

void	print_tokens(const char *input, char **av, t_token_error error)
{
	int	i;

	printf(MAG "Input: %s\n" RESET, input);
	printf(YEL "Error code: %d\n" RESET, error);
	if (!av)
	{
		printf(RED "Returned NULL\n\n" RESET);
		return ;
	}
	i = 0;
	while (av[i] != NULL)
	{
		printf(CYN "av[%d]" RESET " = " GRN "%s\n" RESET, i, av[i]);
		i++;
	}
	printf(CYN "av[%d]" RESET " = " YEL "NULL\n\n" RESET, i);
}

int	main(void)
{
	char			**tokens;
	t_token_error	error;
	int				i;
	int				j;
	const char		*tests[] = {
		"echo hello world",
		"echo \"hello world\"",
		"grep \"foo bar\" file.txt",
		"echo a'b'c",
		"echo a\"b\"c",
		"echo \\\"quoted\\\"",
		"echo '\"hi\"'",
		"echo \"unclosed",
		"echo 'still_unclosed",
		"operator at end <",
		"echo \"this > that\"",
		"echo 'file | grep'",
		"echo 'foo'\"bar\"",
		"echo \"hello\"\"world\"",
		// add more test cases here
		NULL
	};

	printf("=====================TOKENIZER LOOP TEST========================\n");
	i = 0;
	while (tests[i] != NULL)
	{
		error = TOKEN_OK;
		tokens = ft_split_tokens(tests[i], &error);
		print_tokens(tests[i], tokens, error);
		if (tokens)
		{
			j = 0;
			while (tokens[j])
			{
				free(tokens[j]);
				j++;
			}
			free(tokens);
		}
		i++;
	}
	printf("=====================END TOKENIZER LOOP TEST=====================\n");
	return (0);
}
