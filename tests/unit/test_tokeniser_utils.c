#include "minishell.h"

void	test_update_quote_state_basic(void)
{
	t_quote	state;

	printf(BR_BLU "*** TEST 1: update_quote_state basic transitions ***" RESET "\n");

	// test entering single quotes from normal state
	state = STATE_NORMAL;
	state = update_quote_state(state, '\'');
	printf(YEL "[STATE_NORMAL + '] \nexpected: \tSTATE_IN_SINGLE_QUOTE" RESET "\n");
	printf(CYN "actual: \t%s" RESET "\n\n",
		state == STATE_IN_SINGLE_QUOTE ? GRN "STATE_IN_SINGLE_QUOTE" : RED "FAILED");

	// test exiting single quotes back to normal state
	state = update_quote_state(state, '\'');
	printf(YEL "[STATE_IN_SINGLE_QUOTE + '] \nexpected: \tSTATE_NORMAL" RESET "\n");
	printf(CYN "actual: \t%s" RESET "\n\n",
		state == STATE_NORMAL ? GRN "STATE_NORMAL" : RED "FAILED");

	// test entering double quotes from normal state
	state = STATE_NORMAL;
	state = update_quote_state(state, '"');
	printf(YEL "[STATE_NORMAL + \"] \nexpected: \tSTATE_IN_DOUBLE_QUOTE" RESET "\n");
	printf(CYN "actual: \t%s" RESET "\n\n",
		state == STATE_IN_DOUBLE_QUOTE ? GRN "STATE_IN_DOUBLE_QUOTE" : RED "FAILED");

	// test exiting double quotes back to normal state
	state = update_quote_state(state, '"');
	printf(YEL "[STATE_IN_DOUBLE_QUOTE + \"] \nexpected: \tSTATE_NORMAL" RESET "\n");
	printf(CYN "actual: \t%s" RESET "\n\n",
		state == STATE_NORMAL ? GRN "STATE_NORMAL" : RED "FAILED");

	// test double quote inside single quotes should be ignored
	state = STATE_IN_SINGLE_QUOTE;
	state = update_quote_state(state, '"');
	printf(YEL "[STATE_IN_SINGLE_QUOTE + \"] \nexpected: \tSTATE_IN_SINGLE_QUOTE" RESET "\n");
	printf(CYN "actual: \t%s" RESET "\n\n",
		state == STATE_IN_SINGLE_QUOTE ? GRN "STATE_IN_SINGLE_QUOTE" : RED "FAILED");

	// test single quote inside double quotes should be ignored
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

	// test entering double quotes - start of "hello 'nested' quotes"
	state = update_quote_state(state, '"');
	printf(YEL "test: \"hello 'nested' quotes\"" RESET "\n");
	printf(CYN "after \": %s" RESET "\n",
		state == STATE_IN_DOUBLE_QUOTE ? GRN "IN_DOUBLE_QUOTE" : RED "OTHER");

	// test single quote inside double quotes - should stay in double quote state
	state = update_quote_state(state, '\'');
	printf(CYN "after ': %s" RESET "\n",
		state == STATE_IN_DOUBLE_QUOTE ? GRN "still IN_DOUBLE_QUOTE" : RED "FAILED");

	// test closing single quote inside double quotes - should still stay in double quote state
	state = update_quote_state(state, '\'');
	printf(CYN "after ': %s" RESET "\n",
		state == STATE_IN_DOUBLE_QUOTE ? GRN "still IN_DOUBLE_QUOTE" : RED "FAILED");

	// test closing double quote - should return to normal state
	state = update_quote_state(state, '"');
	printf(CYN "after \": %s" RESET "\n",
		state == STATE_NORMAL ? GRN "back to NORMAL" : RED "FAILED");
	printf("\n");
}

void	test_is_whitespace(void)
{
	printf(BR_BLU "*** TEST 3: is_whitespace function ***" RESET "\n");

	// real whitesapces
	char whitespaces[] = {' ', '\t', '\n', '\r', '\f', '\v'};
	int i = 0;

	printf(YEL "testing whitespace characters:" RESET "\n");
	while (i < 6)
	{
		int result = is_whitespace(whitespaces[i]);
		printf(CYN "char %d (ASCII): %s" RESET "\n", whitespaces[i],
			result ? GRN "is whitespace " : RED "not whitespace");
		i++;
	}
	printf("\n");

	// non whitespaces
	printf(YEL "testing non-whitespace characters:" RESET "\n");
	char non_whitespaces[] = {'a', '1', '!', '"', '\''};
	i = 0;
	while (i < 5)
	{
		int result = is_whitespace(non_whitespaces[i]);
		printf(CYN "char '%c': %s" RESET "\n", non_whitespaces[i],
			!result ? GRN "not whitespace" : RED "is whitespace");
		i++;
	}
	printf("\n");
}

void	test_is_a_shell_separator(void)
{
	printf(BR_BLU "*** TEST 4: is_a_shell_separator function ***" RESET "\n");

	// test space in normal state - should be a separator
	bool result = is_a_shell_separator(STATE_NORMAL, ' ');
	printf(YEL "[space + STATE_NORMAL] \nexpected: \ttrue" RESET "\n");
	printf(CYN "actual: \t%s" RESET "\n\n",
		result ? GRN "true" : RED "false");

	// test space inside single quotes - should NOT be a separator
	result = is_a_shell_separator(STATE_IN_SINGLE_QUOTE, ' ');
	printf(YEL "[space + STATE_IN_SINGLE_QUOTE] \nexpected: \tfalse" RESET "\n");
	printf(CYN "actual: \t%s" RESET "\n\n",
		!result ? GRN "false" : RED "true");

	// test space inside double quotes - should NOT be a separator
	result = is_a_shell_separator(STATE_IN_DOUBLE_QUOTE, ' ');
	printf(YEL "[space + STATE_IN_DOUBLE_QUOTE] \nexpected: \tfalse" RESET "\n");
	printf(CYN "actual: \t%s" RESET "\n\n",
		!result ? GRN "false" : RED "true");

	// test regular character in normal state - should NOT be a separator
	result = is_a_shell_separator(STATE_NORMAL, 'a');
	printf(YEL "['a' + STATE_NORMAL] \nexpected: \tfalse" RESET "\n");
	printf(CYN "actual: \t%s" RESET "\n\n",
		!result ? GRN "false" : RED "true");
	printf("\n");
}

void	test_count_shell_tokens_basics(void)
{
	printf(BR_BLU "*** TEST 5: count_shell_tokens function ***" RESET "\n");

	// test simple case
	int count = count_shell_tokens("echo hello");
	printf(YEL "\"echo hello\" \nexpected: \t2" RESET "\n");
	printf(CYN "actual: \t%d %s" RESET "\n\n", count,
		count == 2 ? GRN "PASS" : RED "FAIL");

	// test with quotes
	count = count_shell_tokens("echo \"hello world\"");
	printf(YEL "\"echo \\\"hello world\\\"\" \nexpected: \t2" RESET "\n");
	printf(CYN "actual: \t%d %s" RESET "\n\n", count,
		count == 2 ? GRN "PASS" : RED "FAIL");

	// test with single quotes
	count = count_shell_tokens("echo 'hello world'");
	printf(YEL "\"echo 'hello world'\" \nexpected: \t2" RESET "\n");
	printf(CYN "actual: \t%d %s" RESET "\n\n", count,
		count == 2 ? GRN "PASS" : RED "FAIL");

	// test empty string
	count = count_shell_tokens("");
	printf(YEL "\"\" \nexpected: \t0" RESET "\n");
	printf(CYN "actual: \t%d %s" RESET "\n\n", count,
		count == 0 ? GRN "PASS" : RED "FAIL");

	// test only spaces
	count = count_shell_tokens("   ");
	printf(YEL "\"   \" \nexpected: \t0" RESET "\n");
	printf(CYN "actual: \t%d %s" RESET "\n\n", count,
		count == 0 ? GRN "PASS" : RED "FAIL");
	printf("\n");
}

void	test_count_shell_tokens_advanced(void)
{
	printf(BR_BLU "*** TEST 5B: count_shell_tokens advanced cases ***" RESET "\n");

	// test nested quotes cases
	int count = count_shell_tokens("echo \"hello 'nested' quotes\"");
	printf(YEL "\"echo \\\"hello 'nested' quotes\\\"\" \nexpected: \t2" RESET "\n");
	printf(CYN "actual: \t%d %s" RESET "\n", count,
		count == 2 ? GRN "PASS" : RED "FAIL");

	// test reverse nested quotes
	count = count_shell_tokens("echo 'hello \"nested\" quotes'");
	printf(YEL "\"echo 'hello \\\"nested\\\" quotes'\" \nexpected: \t2" RESET "\n");
	printf(CYN "actual: \t%d %s" RESET "\n", count,
		count == 2 ? GRN "PASS" : RED "FAIL");

	// test multiple tokens with quotes
	count = count_shell_tokens("echo \"hello world\" grep 'test string'");
	printf(YEL "\"echo \\\"hello world\\\" grep 'test string'\" \nexpected: \t4" RESET "\n");
	printf(CYN "actual: \t%d %s" RESET "\n", count,
		count == 4 ? GRN "PASS" : RED "FAIL");

	// test quotes at start and end
	count = count_shell_tokens("\"start with quotes\" and 'end with quotes'");
	printf(YEL "\"\\\"start with quotes\\\" and 'end with quotes'\" \nexpected: \t3" RESET "\n");
	printf(CYN "actual: \t%d %s" RESET "\n", count,
		count == 3 ? GRN "PASS" : RED "FAIL");

	// test consecutive quotes
	count = count_shell_tokens("echo \"first\"\"second\"");
	printf(YEL "\"echo \\\"first\\\"\\\"second\\\"\" \nexpected: \t3" RESET "\n");
	printf(CYN "actual: \t%d %s" RESET "\n", count,
		count == 2 ? GRN "PASS" : RED "FAIL");

	// test mixed quotes with multiple spaces
	count = count_shell_tokens("  echo    \"hello world\"   grep   'test'  ");
	printf(YEL "\"  echo    \\\"hello world\\\"   grep   'test'  \" \nexpected: \t4" RESET "\n");
	printf(CYN "actual: \t%d %s" RESET "\n", count,
		count == 4 ? GRN "PASS" : RED "FAIL");
	printf("\n");
}

int	main(void)
{
	printf(MAG "=== TOKENIZER UTILS FUNCTIONS TESTS ===" RESET "\n\n");

	test_update_quote_state_basic();
	test_update_quote_state_nested();
	test_is_whitespace();
	test_is_a_shell_separator();
	test_count_shell_tokens_basics();
	test_count_shell_tokens_advanced();
	return (0);
}
