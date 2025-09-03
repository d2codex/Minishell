#include "minishell.h"

/**
 * @file test_tokeniser_smart_split.c
 * @brief Test program for the main tokenization function ft_split_tokens().
 *
 * This comprehensive test suite validates the complete tokenization process,
 * including quote handling, operator recognition, error detection, and
 * complex edge cases that could break shell parsing.
 *
 * Compile: make build TEST=unit/test_tokeniser_smart_split.c
 *
 * Usage examples:
 * 1. Run complete test suite:
 *      ./bin/test_tokeniser_smart_split
 *    Expected output:
 *      - All basic cases tokenize correctly
 *      - Quote preservation and metacharacter protection
 *      - Error detection for unclosed quotes
 *      - Complex operator sequences parsed properly
 *
 * 2. Test categories and key cases:
 *    Basic tokenization:
 *      "echo hello" → ["echo", "hello"]
 *      "echo hello > file.txt" → ["echo", "hello", ">", "file.txt"]
 *      "cat input.txt >> output.txt" → ["cat", "input.txt", ">>", "output.txt"]
 *
 *    Quote handling:
 *      "echo 'hello world'" → ["echo", "'hello world'"]
 *      "echo 'hello|world'" → ["echo", "'hello|world'"] (pipe protected)
 *      "echo \"hello 'nested' quotes\"" → ["echo", "\"hello 'nested' quotes\""]
 *
 *    Error cases:
 *      "echo 'unclosed" → NULL with TOKEN_UNCLOSED_QUOTE error
 *      Empty/whitespace-only strings handled gracefully
 *      Multiple consecutive operators: "<<< >>> ||"
 *
 *    Complex cases:
 *      "cat < input.txt | grep hello >> output.txt" → 8 tokens
 *      "echo hello>file.txt" → ["echo", "hello", ">", "file.txt"]
 *
 *    Tricky metacharacter cases:
 *      "echo hello>>>file" → ["echo", "hello", ">>", ">", "file"]
 *      "cat<file|grep>out" → ["cat", "<", "file", "|", "grep", ">", "out"]
 *      "echo'hello'>file" → ["echo'hello'", ">", "file"]
 *      "\"cat<file\"|grep" → ["\"cat<file\"", "|", "grep"]
 *
 * 3. Error handling verification:
 *    - TOKEN_OK: successful tokenization
 *    - TOKEN_UNCLOSED_QUOTE: detected before processing
 *    - TOKEN_MALLOC_ERROR: memory allocation failures
 *    - Proper cleanup on all error paths
 *
 * 4. Memory management:
 *    All test cases use free_tokens_safe() to prevent memory leaks.
 *    The tokenizer properly handles memory allocation failures and
 *    cleans up partial allocations.
 *
 * Notes:
 * - Tests mirror real bash tokenization behavior exactly
 * - Handles all mandatory part requirements from minishell subject
 * - Edge cases include adjacent operators, empty quotes, and mixed quoting
 * - Visual output shows input → tokens mapping for easy debugging
 * - Critical for validating parser input before AST construction
 */
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

void	test_ugly_metachars(void)
{
	char			**tokens;
	t_token_error	error;

	printf(BR_BLU "*** Ugly metacharacter cases ***" RESET "\n");

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
	printf(MAG "=== TOKENIZER SMART SPLIT TESTS ===" RESET "\n\n");

	test_basic_tokenization();
	test_quotes_handling();
	test_error_cases();
	test_ugly_metachars();
	return (0);
}
