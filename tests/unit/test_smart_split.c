#include "minishell.h"

/* wrapper to be able to use my free_tab function that needs a count param */
void	free_tokens(char **tokens)
{
	int	count;

	if (!tokens)
		return;
	count = 0;
	while (tokens[count])
		count++;
	free_tab(tokens, count);
}

void	print_tokens(char **tokens)
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

void	test_simple_cases(void)
{
	char	**tokens;

	printf(BR_BLU "*** Basic tokenization ***" RESET "\n");

	// Test 1: simple case
	tokens = ft_split_tokens("echo hello");
	printf(YEL "Input: " RESET "\"echo hello\"\n");
	printf(CYN "Tokens: " RESET);
	print_tokens(tokens);
	printf("\n\n");
	free_tokens(tokens);

	// Test 2: multiple spaces
	tokens = ft_split_tokens("   echo   hello   world   ");
	printf(YEL "Input: " RESET "\"   echo   hello   world   \"\n");
	printf(CYN "Tokens: " RESET);
	print_tokens(tokens);
	printf("\n\n");
	free_tokens(tokens);

	// Test 3: empty string
	tokens = ft_split_tokens("");
	printf(YEL "Input: " RESET "\"\"\n");
	printf(CYN "Tokens: " RESET);
	print_tokens(tokens);
	printf("\n\n");
	free_tokens(tokens);
}

void	test_quotes(void)
{
	char	**tokens;

	printf(BR_BLU "*** Quote handling ***" RESET "\n");

	// Test 1: double quotes
	tokens = ft_split_tokens("echo \"hello world\"");
	printf(YEL "Input: " RESET "\"echo \\\"hello world\\\"\"\n");
	printf(CYN "Tokens: " RESET);
	print_tokens(tokens);
	printf("\n\n");
	free_tokens(tokens);

	// Test 2: dingle quotes
	tokens = ft_split_tokens("echo 'hello world'");
	printf(YEL "Input: " RESET "\"echo 'hello world'\"\n");
	printf(CYN "Tokens: " RESET);
	print_tokens(tokens);
	printf("\n\n");
	free_tokens(tokens);

	// Test 3: nested quotes
	tokens = ft_split_tokens("echo \"hello 'world'\"");
	printf(YEL "Input: " RESET "\"echo \\\"hello 'world'\\\"\"\n");
	printf(CYN "Tokens: " RESET);
	print_tokens(tokens);
	printf("\n\n");
	free_tokens(tokens);

	// Test 4: concatenated quotes
	tokens = ft_split_tokens("echo\"hello\"world");
	printf(YEL "Input: " RESET "\"echo\\\"hello\\\"world\"\n");
	printf(CYN "Tokens: " RESET);
	print_tokens(tokens);
	printf("\n\n");
	free_tokens(tokens);
}

void	test_error_cases(void)
{
	char	**tokens;

	printf(BR_BLU "*** Error cases ***" RESET "\n");

	// Test 1: unclosed single quote
	tokens = ft_split_tokens("echo 'unclosed");
	printf(YEL "Input: " RESET "\"echo 'unclosed\"\n");
	printf(CYN "Tokens: " RESET);
	print_tokens(tokens);
	if (!tokens)
		printf(" " GRN "(correctly rejected)" RESET);
	printf("\n\n");
	free_tokens(tokens);

	// Test 2: unclosed double quote
	tokens = ft_split_tokens("echo \"unclosed");
	printf(YEL "Input: " RESET "\"echo \\\"unclosed\"\n");
	printf(CYN "Tokens: " RESET);
	print_tokens(tokens);
	if (!tokens)
		printf(" " GRN "(correctly rejected)" RESET);
	printf("\n\n");
	free_tokens(tokens);

	// Test 3: mixed unclosed quotes
	tokens = ft_split_tokens("echo 'mixed \" unclosed");
	printf(YEL "Input: " RESET "\"echo 'mixed \\\" unclosed\"\n");
	printf(CYN "Tokens: " RESET);
	print_tokens(tokens);
	if (!tokens)
		printf(" " GRN "(correctly rejected)" RESET);
	printf("\n\n");
	free_tokens(tokens);
}

void	test_edge_cases(void)
{
	char	**tokens;

	printf(BR_BLU "*** Edge cases ***" RESET "\n");

	// Test 1: empty quotes
	tokens = ft_split_tokens("echo '' \"\"");
	printf(YEL "Input: " RESET "\"echo '' \\\"\\\"\"\n");
	printf(CYN "Tokens: " RESET);
	print_tokens(tokens);
	printf("\n\n");
	free_tokens(tokens);

	// Test 2: only spaces in quotes
	tokens = ft_split_tokens("echo '   ' \"   \"");
	printf(YEL "Input: " RESET "\"echo '   ' \\\"   \\\"\"\n");
	printf(CYN "Tokens: " RESET);
	print_tokens(tokens);
	printf("\n\n");
	free_tokens(tokens);

	// Test 3: quote characters inside quotes
	tokens = ft_split_tokens("echo '\"test\"' \"'test'\"");
	printf(YEL "Input: " RESET "\"echo '\\\"test\\\"' \\\"'test'\\\"\"\n");
	printf(CYN "Tokens: " RESET);
	print_tokens(tokens);
	printf("\n\n");
	free_tokens(tokens);
}

int	main(void)
{
	printf(MAG "=== TOKENIZER TESTS ===" RESET "\n\n");

	test_simple_cases();
	test_quotes();
	test_error_cases();
	test_edge_cases();

	return (0);
}
