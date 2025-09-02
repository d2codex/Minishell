#include "minishell.h"

void	free_tokens_safe(char **tokens)
{
	int	count;

	if (!tokens)
		return;
	count = 0;
	while (tokens[count])
		count++;
	free_string_array(tokens, count);
}

void	print_tokens_safe(char **tokens)
{
	int	i;

	if (!tokens)
	{
		printf(RED "NULL" RESET);
		return;
	}

	printf("[");
	i = 0;
	while (tokens[i])
	{
		printf("\"%s\"", tokens[i]);
		if (tokens[i + 1])
			printf(", ");
		i++;
	}
	printf("]");
}

void	test_basic_tokenization(void)
{
	char			**tokens;
	t_token_error	error;

	printf(BR_BLU "*** Basic cases ***" RESET "\n");

	error = TOKEN_OK;
	tokens = ft_split_tokens("echo hello", &error);
	printf(YEL "Input: " RESET "\"echo hello\"\n");
	printf(CYN "Tokens: " RESET);
	print_tokens_safe(tokens);
	printf(CYN " (error: %d)" RESET "\n\n", error);
	free_tokens_safe(tokens);

	error = TOKEN_OK;
	tokens = ft_split_tokens("echo hello > file.txt", &error);
	printf(YEL "Input: " RESET "\"echo hello > file.txt\"\n");
	printf(CYN "Tokens: " RESET);
	print_tokens_safe(tokens);
	printf(CYN " (error: %d)" RESET "\n\n", error);
	free_tokens_safe(tokens);

	error = TOKEN_OK;
	tokens = ft_split_tokens("cat input.txt >> output.txt", &error);
	printf(YEL "Input: " RESET "\"cat input.txt >> output.txt\"\n");
	printf(CYN "Tokens: " RESET);
	print_tokens_safe(tokens);
	printf(CYN " (error: %d)" RESET "\n\n", error);
	free_tokens_safe(tokens);
}

void	test_quotes_handling(void)
{
	char			**tokens;
	t_token_error	error;

	printf(BR_BLU "*** Quote handling ***" RESET "\n");

	error = TOKEN_OK;
	tokens = ft_split_tokens("echo 'hello world'", &error);
	printf(YEL "Input: " RESET "\"echo 'hello world'\"\n");
	printf(CYN "Tokens: " RESET);
	print_tokens_safe(tokens);
	printf(CYN " (error: %d)" RESET "\n\n", error);
	free_tokens_safe(tokens);

	error = TOKEN_OK;
	tokens = ft_split_tokens("echo 'hello|world'", &error);
	printf(YEL "Input: " RESET "\"echo 'hello|world'\"\n");
	printf(CYN "Tokens: " RESET);
	print_tokens_safe(tokens);
	printf(CYN " (error: %d)" RESET "\n\n", error);
	free_tokens_safe(tokens);

	error = TOKEN_OK;
	tokens = ft_split_tokens("echo \"hello 'nested' quotes\"", &error);
	printf(YEL "Input: " RESET "\"echo \\\"hello 'nested' quotes\\\"\"\n");
	printf(CYN "Tokens: " RESET);
	print_tokens_safe(tokens);
	printf(CYN " (error: %d)" RESET "\n\n", error);
	free_tokens_safe(tokens);
}

void	test_error_cases(void)
{
	char			**tokens;
	t_token_error	error;

	printf(BR_BLU "*** Error cases ***" RESET "\n");

	error = TOKEN_OK;
	tokens = ft_split_tokens("echo 'unclosed", &error);
	printf(YEL "Input: " RESET "\"echo 'unclosed\"\n");
	printf(CYN "Tokens: " RESET);
	print_tokens_safe(tokens);
	printf(CYN " (error: %d)" RESET, error);
	if (error == TOKEN_UNCLOSED_QUOTE)
		printf(GRN " - correctly detected" RESET);
	printf("\n\n");
	free_tokens_safe(tokens);

	error = TOKEN_OK;
	tokens = ft_split_tokens("", &error);
	printf(YEL "Input: " RESET "\"\"\n");
	printf(CYN "Tokens: " RESET);
	print_tokens_safe(tokens);
	printf(CYN " (error: %d)" RESET "\n\n", error);
	free_tokens_safe(tokens);

	error = TOKEN_OK;
	tokens = ft_split_tokens("echo	 hello   world", &error);
	printf(YEL "Input: " RESET "\"echo	 hello   world\"\n");
	printf(CYN "Tokens: " RESET);
	print_tokens_safe(tokens);
	printf(CYN " (error: %d)" RESET "\n\n", error);
	free_tokens_safe(tokens);

	error = TOKEN_OK;
	tokens = ft_split_tokens("echo \"\" ''", &error);
	printf(YEL "Input: " RESET "\"echo \"\" ''\"\n");
	printf(CYN "Tokens: " RESET);
	print_tokens_safe(tokens);
	printf(CYN " (error: %d)" RESET "\n\n", error);
	free_tokens_safe(tokens);

	error = TOKEN_OK;
	tokens = ft_split_tokens("<<< >>> ||", &error);
	printf(YEL "Input: " RESET "\"<<< >>> ||\"\n");
	printf(CYN "Tokens: " RESET);
	print_tokens_safe(tokens);
	printf(CYN " (error: %d)" RESET "\n\n", error);
	free_tokens_safe(tokens);
}

void	test_complex_cases(void)
{
	char			**tokens;
	t_token_error	error;

	printf(BR_BLU "*** Complex cases ***" RESET "\n");

	error = TOKEN_OK;
	tokens = ft_split_tokens("cat < input.txt | grep hello >> output.txt", &error);
	printf(YEL "Input: " RESET "\"cat < input.txt | grep hello >> output.txt\"\n");
	printf(CYN "Tokens: " RESET);
	print_tokens_safe(tokens);
	printf(CYN " (error: %d)" RESET "\n\n", error);
	free_tokens_safe(tokens);

	error = TOKEN_OK;
	tokens = ft_split_tokens("echo hello>file.txt", &error);
	printf(YEL "Input: " RESET "\"echo hello>file.txt\"\n");
	printf(CYN "Tokens: " RESET);
	print_tokens_safe(tokens);
	printf(CYN " (error: %d)" RESET "\n\n", error);
	free_tokens_safe(tokens);
}

void	test_ugly_metachars(void)
{
	char			**tokens;
	t_token_error	error;

	printf(BR_BLU "*** Tricky metacharacter cases ***" RESET "\n");

	error = TOKEN_OK;
	tokens = ft_split_tokens("echo hello>>>file", &error);
	printf(YEL "Input: " RESET "\"echo hello>>>file\"\n");
	printf(CYN "Tokens: " RESET);
	print_tokens_safe(tokens);
	printf(CYN " (error: %d)" RESET "\n\n", error);
	free_tokens_safe(tokens);

	error = TOKEN_OK;
	tokens = ft_split_tokens("cat<file|grep>out", &error);
	printf(YEL "Input: " RESET "\"cat<file|grep>out\"\n");
	printf(CYN "Tokens: " RESET);
	print_tokens_safe(tokens);
	printf(CYN " (error: %d)" RESET "\n\n", error);
	free_tokens_safe(tokens);

	error = TOKEN_OK;
	tokens = ft_split_tokens(">echo hello<", &error);
	printf(YEL "Input: " RESET "\">echo hello<\"\n");
	printf(CYN "Tokens: " RESET);
	print_tokens_safe(tokens);
	printf(CYN " (error: %d)" RESET "\n\n", error);
	free_tokens_safe(tokens);

	error = TOKEN_OK;
	tokens = ft_split_tokens(">><<||><", &error);
	printf(YEL "Input: " RESET "\">><<||><\"\n");
	printf(CYN "Tokens: " RESET);
	print_tokens_safe(tokens);
	printf(CYN " (error: %d)" RESET "\n\n", error);
	free_tokens_safe(tokens);

	error = TOKEN_OK;
	tokens = ft_split_tokens("echo'hello'>file", &error);
	printf(YEL "Input: " RESET "\"echo'hello'>file\"\n");
	printf(CYN "Tokens: " RESET);
	print_tokens_safe(tokens);
	printf(CYN " (error: %d)" RESET "\n\n", error);
	free_tokens_safe(tokens);

	error = TOKEN_OK;
	tokens = ft_split_tokens("\"cat<file\"|grep", &error);
	printf(YEL "Input: " RESET "\"\\\"cat<file\\\"|grep\"\n");
	printf(CYN "Tokens: " RESET);
	print_tokens_safe(tokens);
	printf(CYN " (error: %d)" RESET "\n\n", error);
	free_tokens_safe(tokens);

	error = TOKEN_OK;
	tokens = ft_split_tokens("echo>>>\"file>name\"|grep", &error);
	printf(YEL "Input: " RESET "\"echo>>>\\\"file>name\\\"|grep\"\n");
	printf(CYN "Tokens: " RESET);
	print_tokens_safe(tokens);
	printf(CYN " (error: %d)" RESET "\n\n", error);
	free_tokens_safe(tokens);

	error = TOKEN_OK;
	tokens = ft_split_tokens("><|<>", &error);
	printf(YEL "Input: " RESET "\" ><|<> \"\n");
	printf(CYN "Tokens: " RESET);
	print_tokens_safe(tokens);
	printf(CYN " (error: %d)" RESET "\n\n", error);
	free_tokens_safe(tokens);
}

int	main(void)
{
	printf(MAG "=== TOKENIZER TESTS ===" RESET "\n\n");

	test_basic_tokenization();
	test_quotes_handling();
	test_error_cases();
	test_complex_cases();
	test_ugly_metachars();
	return (0);
}
