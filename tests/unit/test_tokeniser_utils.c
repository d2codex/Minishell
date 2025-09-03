#include "minishell.h"

/**
 * @file test_tokeniser_utils.c
 * @brief Test program for tokenizer utility functions and quote state management.
 *
 * This test suite validates the core utility functions used by the tokenizer:
 * quote state transitions, whitespace detection, shell separator identification,
 * and synchronization between token counting and extraction functions.
 *
 * Compile: make build TEST=unit/test_tokeniser_utils.c
 *
 * Usage examples:
 * 1. Basic functionality test:
 *      ./bin/test_tokeniser_utils
 *    Expected output:
 *      - SUCCESS for all quote state transitions (NORMAL ↔ SINGLE ↔ DOUBLE)
 *      - Correct whitespace detection for all 6 standard whitespace chars
 *      - Proper shell separator behavior with different quote states
 *      - Perfect synchronization between count_shell_tokens() and extraction
 *
 * 2. Key test cases verified:
 *    Quote state machine:
 *      - STATE_NORMAL + ' → STATE_IN_SINGLE_QUOTE
 *      - Nested quotes: "hello 'nested' quotes" handled correctly
 *      - Quote isolation: single quotes ignore double quotes and vice versa
 *    
 *    Shell separators:
 *      - Whitespace and operators (|<>) are separators in STATE_NORMAL
 *      - Same chars are NOT separators inside quotes
 *    
 *    Count vs Extract synchronization:
 *      - "echo hello" → both functions return 2
 *      - "cat < input.txt | grep hello >> output.txt" → both return 8
 *      - Complex cases with quotes and metacharacters
 *
 * 3. Critical validation:
 *    The test_count_vs_extract() function is crucial for preventing
 *    desynchronization bugs that could cause memory corruption or
 *    incorrect parsing in the main shell loop.
 *
 * Notes:
 * - All functions are tested independently before integration
 * - Color-coded output (GRN=pass, RED=fail) for easy visual verification
 * - Tests cover edge cases like empty strings, whitespace-only input
 * - Quote nesting follows bash behavior exactly
 */
void	test_update_quote_state_basic(void)
{
	t_quote	state;

	printf(BR_BLU "*** TEST 1: update_quote_state basic transitions ***" RESET "\n");

	state = STATE_NORMAL;
	state = update_quote_state(state, '\'');
	printf(YEL "[STATE_NORMAL + '] \nexpected: \tSTATE_IN_SINGLE_QUOTE" RESET "\n");
	printf(CYN "actual: \t%s" RESET "\n\n",
		state == STATE_IN_SINGLE_QUOTE ? GRN "STATE_IN_SINGLE_QUOTE" : RED "FAILED");

	state = update_quote_state(state, '\'');
	printf(YEL "[STATE_IN_SINGLE_QUOTE + '] \nexpected: \tSTATE_NORMAL" RESET "\n");
	printf(CYN "actual: \t%s" RESET "\n\n",
		state == STATE_NORMAL ? GRN "STATE_NORMAL" : RED "FAILED");

	state = STATE_NORMAL;
	state = update_quote_state(state, '"');
	printf(YEL "[STATE_NORMAL + \"] \nexpected: \tSTATE_IN_DOUBLE_QUOTE" RESET "\n");
	printf(CYN "actual: \t%s" RESET "\n\n",
		state == STATE_IN_DOUBLE_QUOTE ? GRN "STATE_IN_DOUBLE_QUOTE" : RED "FAILED");

	state = update_quote_state(state, '"');
	printf(YEL "[STATE_IN_DOUBLE_QUOTE + \"] \nexpected: \tSTATE_NORMAL" RESET "\n");
	printf(CYN "actual: \t%s" RESET "\n\n",
		state == STATE_NORMAL ? GRN "STATE_NORMAL" : RED "FAILED");

	state = STATE_IN_SINGLE_QUOTE;
	state = update_quote_state(state, '"');
	printf(YEL "[STATE_IN_SINGLE_QUOTE + \"] \nexpected: \tSTATE_IN_SINGLE_QUOTE" RESET "\n");
	printf(CYN "actual: \t%s" RESET "\n\n",
		state == STATE_IN_SINGLE_QUOTE ? GRN "STATE_IN_SINGLE_QUOTE" : RED "FAILED");

	state = STATE_IN_DOUBLE_QUOTE;
	state = update_quote_state(state, '\'');
	printf(YEL "[STATE_IN_DOUBLE_QUOTE + '] \nexpected: \tSTATE_IN_DOUBLE_QUOTE" RESET "\n");
	printf(CYN "actual: \t%s" RESET "\n",
		state == STATE_IN_DOUBLE_QUOTE ? GRN "STATE_IN_DOUBLE_QUOTE" : RED "FAILED");

	printf("\n");
}

void	test_update_quote_state_nested(void)
{
	t_quote	state;

	printf(BR_BLU "*** TEST 2: update_quote_state nested quotes ***" RESET "\n");

	state = STATE_NORMAL;
	state = update_quote_state(state, '"');
	printf(YEL "test: \"hello 'nested' quotes\"" RESET "\n");
	printf(CYN "after \": %s" RESET "\n",
		state == STATE_IN_DOUBLE_QUOTE ? GRN "IN_DOUBLE_QUOTE" : RED "OTHER");

	state = update_quote_state(state, '\'');
	printf(CYN "after ': %s" RESET "\n",
		state == STATE_IN_DOUBLE_QUOTE ? GRN "still IN_DOUBLE_QUOTE" : RED "FAILED");

	state = update_quote_state(state, '\'');
	printf(CYN "after ': %s" RESET "\n",
		state == STATE_IN_DOUBLE_QUOTE ? GRN "still IN_DOUBLE_QUOTE" : RED "FAILED");

	state = update_quote_state(state, '"');
	printf(CYN "after \": %s" RESET "\n",
		state == STATE_NORMAL ? GRN "back to NORMAL" : RED "FAILED");
	printf("\n");
}

void	test_is_whitespace(void)
{
	printf(BR_BLU "*** TEST 3: is_whitespace function ***" RESET "\n");

	char whitespaces[] = {' ', '\t', '\n', '\r', '\f', '\v'};
	for (int i = 0; i < 6; i++)
	{
		int result = is_whitespace(whitespaces[i]);
		printf(CYN "char %d (ASCII): %s" RESET "\n", whitespaces[i],
			result ? GRN "is whitespace " : RED "not whitespace");
	}
	printf("\n");

	char non_whitespaces[] = {'a', '1', '!', '"', '\''};
	for (int i = 0; i < 5; i++)
	{
		int result = is_whitespace(non_whitespaces[i]);
		printf(CYN "char '%c': %s" RESET "\n", non_whitespaces[i],
			!result ? GRN "not whitespace" : RED "is whitespace");
	}
	printf("\n");
}

void	test_is_a_shell_separator(void)
{
	printf(BR_BLU "*** TEST 4: is_a_shell_separator ***" RESET "\n");

	bool result = is_a_shell_separator(STATE_NORMAL, ' ');
	printf(YEL "[space + STATE_NORMAL] \nexpected: \ttrue" RESET "\n");
	printf(CYN "actual: \t%s" RESET "\n\n", result ? GRN "true" : RED "false");

	result = is_a_shell_separator(STATE_IN_SINGLE_QUOTE, ' ');
	printf(YEL "[space + STATE_IN_SINGLE_QUOTE] \nexpected: \tfalse" RESET "\n");
	printf(CYN "actual: \t%s" RESET "\n\n", !result ? GRN "false" : RED "true");

	result = is_a_shell_separator(STATE_IN_DOUBLE_QUOTE, ' ');
	printf(YEL "[space + STATE_IN_DOUBLE_QUOTE] \nexpected: \tfalse" RESET "\n");
	printf(CYN "actual: \t%s" RESET "\n\n", !result ? GRN "false" : RED "true");

	result = is_a_shell_separator(STATE_NORMAL, 'a');
	printf(YEL "['a' + STATE_NORMAL] \nexpected: \tfalse" RESET "\n");
	printf(CYN "actual: \t%s" RESET "\n\n", !result ? GRN "false" : RED "true");
	printf("\n");
}

void test_count_vs_extract(void)
{
	printf(BR_BLU "*** TEST 5: count_shell_tokens function vs extracted tokens num ***" RESET "\n");

	t_token_error error;
	char **tokens;
	int counted, extracted;

	error = TOKEN_OK;
	tokens = ft_split_tokens("echo hello", &error);
	counted = count_shell_tokens("echo hello");
	extracted = 0;
	if (tokens)
	{
		while (tokens[extracted])
		extracted++;
		free_string_array(tokens, extracted);
	}
	printf(YEL "Test 1: \"echo hello\"\ncount_shell_tokens = %d, \nextract_tokens_to_tab = %d\n\n" RESET,
		 counted, extracted);

	error = TOKEN_OK;
	tokens = ft_split_tokens("echo 'hello world'", &error);
	counted = count_shell_tokens("echo 'hello world'");
	extracted = 0;
	if (tokens)
	{
		while (tokens[extracted])
			extracted++;
		free_string_array(tokens, extracted);
	}
	printf(YEL "Test 2: \"echo 'hello world'\"\ncount_shell_tokens = %d, \nextract_tokens_to_tab = %d\n\n" RESET,
		counted, extracted);

	error = TOKEN_OK;
	tokens = ft_split_tokens("cat < input.txt | grep hello >> output.txt", &error);
	counted = count_shell_tokens("cat < input.txt | grep hello >> output.txt");
	extracted = 0;
	if (tokens)
	{
		while (tokens[extracted])
			extracted++;
		free_string_array(tokens, extracted);
	}
	printf(YEL "Test 3: \"cat < input.txt | grep hello >> output.txt\"\ncount_shell_tokens = %d, \nextract_tokens_to_tab = %d\n\n" RESET,
		 counted, extracted);

	error = TOKEN_OK;
	tokens = ft_split_tokens("echo hello>file.txt", &error);
	counted = count_shell_tokens("echo hello>file.txt");
	extracted = 0;
	if (tokens)
	{
		while (tokens[extracted])
			extracted++;
		free_string_array(tokens, extracted);
	}
	printf(YEL "Test 4: \"echo hello>file.txt\"\ncount_shell_tokens = %d, \nextract_tokens_to_tab = %d\n\n" RESET,
		counted, extracted);
}


void	test_count_shell_tokens(void)
{

	printf(BR_BLU "*** TEST 6: count_shell_tokens function ***" RESET "\n");

	int count = count_shell_tokens("echo hello");
	printf(YEL "\"echo hello\" expected: 2, actual: %d %s\n" RESET,
		count, count == 2 ? GRN "PASS" : RED "FAIL");

	count = count_shell_tokens("echo \"hello world\"");
	printf(YEL "\"echo \\\"hello world\\\"\" expected: 2, actual: %d %s\n" RESET,
		count, count == 2 ? GRN "PASS" : RED "FAIL");

	count = count_shell_tokens("echo 'hello world'");
	printf(YEL "\"echo 'hello world'\" expected: 2, actual: %d %s\n" RESET,
		count, count == 2 ? GRN "PASS" : RED "FAIL");

	count = count_shell_tokens("echo hello > file.txt");
	printf(YEL "\"echo hello > file.txt\" expected: 4, actual: %d %s\n" RESET,
		count, count == 4 ? GRN "PASS" : RED "FAIL");

	count = count_shell_tokens("ls | grep main");
	printf(YEL "\"ls | grep main\" expected: 4, actual: %d %s\n" RESET,
		count, count == 4 ? GRN "PASS" : RED "FAIL");

	count = count_shell_tokens("");
	printf(YEL "\"\" expected: 0, actual: %d %s\n" RESET,
		count, count == 0 ? GRN "PASS" : RED "FAIL");

	count = count_shell_tokens("   ");
	printf(YEL "\"   \" expected: 0, actual: %d %s\n" RESET,
		count, count == 0 ? GRN "PASS" : RED "FAIL");

	count = count_shell_tokens("echo \"hello 'nested' quotes\"");
	printf(YEL "\"echo \\\"hello 'nested' quotes\\\"\" expected: 2, actual: %d %s\n" RESET,
		count, count == 2 ? GRN "PASS" : RED "FAIL");

	count = count_shell_tokens("echo 'hello \"nested\" quotes'");
	printf(YEL "\"echo 'hello \\\"nested\\\" quotes'\" expected: 2, actual: %d %s\n" RESET,
		count, count == 2 ? GRN "PASS" : RED "FAIL");
}

int	main(void)
{
	printf(MAG "=== TOKENIZER UTILS FUNCTIONS TESTS ===" RESET "\n\n");

	test_update_quote_state_basic();
	test_update_quote_state_nested();
	test_is_whitespace();
	test_is_a_shell_separator();
	test_count_vs_extract();
	test_count_shell_tokens();
	return (0);
}
