#include "minishell.h"

/**
 * @file test_builtin_exit.c
 * @brief Unit tests for the exit builtin command in minishell.
 *
 * @details
 * This file contains individual unit tests for the exit builtin.
 * Each test function represents a single scenario to verify correct
 * behavior of the builtin, including:
 *   - No arguments (uses last command status)
 *   - Valid numeric argument (with modulo 256 handling)
 *   - Invalid numeric argument (prints error, sets status = 2)
 *   - Too many arguments (prints error, sets status = 1 but shell continues)
 *   - Edge cases: negative numbers, large numbers, empty strings, trailing characters
 *
 * Test methodology follows Test Driven Development (TDD):
 * 1. Write one failing test
 * 2. Implement minimal code to make it pass
 * 3. Refactor if needed
 * 4. Repeat with the next case
 *
 * @note
 * Compile these tests with:
 *	make build TEST=unit/test_builtin_exit.c
 * valgrind:
 *	make valgrind TEST=unit/test_builtin_exit.c
 */
void test_exit_no_args(void)
{
	printf(BR_BLU "TEST 1: exit without arguments - [exit]\n" RESET);
	// we simulate a stored status from previous command - to replicate in bash:
	// bash
	// (exit 42)    # puts $? = 42 without leaving bash
	// exit
	char *tokens[] = {"exit", NULL};
	t_shell data = {
		.env_list = NULL,
		.status = 42,
		.is_tty = true,
		.is_child = false
	};

	printf("\ncontext: exit + no argument: write 'exit\\n' & 'echo $?' will use last known status code\n\n");
	printf("builtin_exit result:\n");
	int result = builtin_exit(tokens, &data);
	printf("bash behavior:\n");
	printf("exit\n");
	printf(YEL "\nexpected return:\t-1 (exit signal)\n" RESET);
	printf(YEL "actual return:\t\t%d\n" RESET, result);
	printf(YEL "expected status:\t42 (unchanged)\n" RESET);
	printf(YEL "actual status:\t\t%d\n" RESET, data.status);
	printf("RESULT: %s\n\n",
		(result == -1 && data.status == 42) ? GRN "PASS" RESET : RED "FAIL" RESET);
}

void test_exit_valid_number(void)
{
	printf(BR_BLU "TEST 2: exit with valid number - [exit 123]\n" RESET);

	char *tokens[] = {"exit", "123", NULL};
	t_shell data = {
		.env_list = NULL,
		.status = 42,
		.is_tty = true,
		.is_child = false
	};

	printf("\ncontext: exit + valid numeric argument: write 'exit\\n', when user types 'echo $?' it shows provided code\n\n");
	printf("builtin_exit result:\n");
	int result = builtin_exit(tokens, &data);
	printf("bash behavior:\n");
	printf("exit\n");
	printf(YEL "\nexpected return:\t-1 (exit signal)\n" RESET);
	printf(YEL "actual return:\t\t%d\n" RESET, result);
	printf(YEL "expected status:\t123\n" RESET);
	printf(YEL "actual status:\t\t%d\n" RESET, data.status);
	printf("RESULT: %s\n\n",
		(result == -1 && data.status == 123) ? GRN "PASS" RESET : RED "FAIL" RESET);
}

void test_exit_too_many_args(void)
{
	printf(BR_BLU "TEST 3: exit with too many arguments - [exit 123 456 hello]\n" RESET);

	char *tokens[] = {"exit", "123", "456", "hello", NULL};
	t_shell data = {
		.env_list = NULL,
		.status = 42,
		.is_tty = true,
		.is_child = false
	};

	printf("\ncontext: exit + too many args: write 'exit\\n' + error message & continue shell\n\n");
	printf("builtin_exit result:\n");
	int result = builtin_exit(tokens, &data);
	printf("bash behavior:\n");
	printf("exit\n");
	printf("bash: exit: too many arguments\n");
	printf(YEL "\nexpected return:\t0 (continue shell)\n" RESET);
	printf(YEL "actual return:\t\t%d\n" RESET, result);
	printf(YEL "expected status:\t1\n" RESET);
	printf(YEL "actual status:\t\t%d\n" RESET, data.status);
	printf("RESULT: %s\n\n",
		(result == 0 && data.status == 1) ? GRN "PASS" RESET : RED "FAIL" RESET);
}

void test_exit_invalid_arg(void)
{
	printf(BR_BLU "TEST 4: exit with invalid argument- [exit hello]\n " RESET);

	char *tokens[] = {"exit", "hello", NULL};
	t_shell data = {
		.env_list = NULL,
		.status = 42,
		.is_tty = true,
		.is_child = false
	};

	printf("\ncontext: exit + invalid arg: write 'exit\\n' + error message & terminate shell\n\n");
	printf("builtin_exit result:\n");
	int result = builtin_exit(tokens, &data);
	printf("bash behavior:\n");
	printf("exit\n");
	printf("bash: exit: hello: numeric argument required\n");
	printf(YEL "\nexpected return:\t-1 (exit signal)\n" RESET);
	printf(YEL "actual return:\t\t%d\n" RESET, result);
	printf(YEL "expected status:\t2\n" RESET);
	printf(YEL "actual status:\t\t%d\n" RESET, data.status);
	printf("RESULT: %s\n\n",
		(result == -1 && data.status == 2) ? GRN "PASS" RESET : RED "FAIL" RESET);
}

void test_exit_modulo_256(void)
{
	printf(BR_BLU "TEST 5: exit with number > 255 - [exit 999]\n" RESET);

	char *tokens[] = {"exit", "999", NULL};
	t_shell data = {
		.env_list = NULL,
		.status = 42,
		.is_tty = true,
		.is_child = false
	};

	printf("\ncontext: exit + large number: modulo 256 conversion (999 %% 256 = 231)\n\n");
	printf("builtin_exit result:\n");
	int result = builtin_exit(tokens, &data);
	printf("bash behavior:\n");
	printf("exit\n");
	printf(YEL "\nexpected return:\t-1 (exit signal)\n" RESET);
	printf(YEL "actual return:\t\t%d\n" RESET, result);
	printf(YEL "expected status:\t231\n" RESET);
	printf(YEL "actual status:\t\t%d\n" RESET, data.status);
	printf("RESULT: %s\n\n",
		(result == -1 && data.status == 231) ? GRN "PASS" RESET : RED "FAIL" RESET);
}

void test_exit_negative_number(void)
{
	printf(BR_BLU "TEST 6: exit with negative number - [exit -1]\n" RESET);

	char *tokens[] = {"exit", "-1", NULL};
	t_shell data = {
		.env_list = NULL,
		.status = 42,
		.is_tty = true,
		.is_child = false
	};

	printf("\ncontext: exit + negative number: unsigned char conversion (-1 becomes 255)\n\n");
	printf("builtin_exit result:\n");
	int result = builtin_exit(tokens, &data);
	printf("bash behavior:\n");
	printf("exit\n");
	printf(YEL "\nexpected return:\t-1 (exit signal)\n" RESET);
	printf(YEL "actual return:\t\t%d\n" RESET, result);
	printf(YEL "expected status:\t255\n" RESET);
	printf(YEL "actual status:\t\t%d\n" RESET, data.status);
	printf("RESULT: %s\n\n",
		(result == -1 && data.status == 255) ? GRN "PASS" RESET : RED "FAIL" RESET);
}

void test_exit_overflow(void)
{
	printf(BR_BLU "TEST 7: exit with overflow number - [exit 999999999999999999999999]\n" RESET);

	char *tokens[] = {"exit", "999999999999999999999999", NULL};
	t_shell data = {
		.env_list = NULL,
		.status = 42,
		.is_tty = true,
		.is_child = false
	};

	printf("\ncontext: exit + overflow: number exceeds LLONG_MAX, treated as invalid\n\n");
	printf("builtin_exit result:\n");
	int result = builtin_exit(tokens, &data);
	printf("bash behavior:\n");
	printf("exit\n");
	printf("bash: exit: 999999999999999999999999: numeric argument required\n");
	printf(YEL "\nexpected return:\t-1 (exit signal)\n" RESET);
	printf(YEL "actual return:\t\t%d\n" RESET, result);
	printf(YEL "expected status:\t2\n" RESET);
	printf(YEL "actual status:\t\t%d\n" RESET, data.status);
	printf("RESULT: %s\n\n",
		(result == -1 && data.status == 2) ? GRN "PASS" RESET : RED "FAIL" RESET);
}

void test_exit_empty_string(void)
{
	printf(BR_BLU "TEST 8: exit with empty string - [exit empty string]\n" RESET);

	char *tokens[] = {"exit", "", NULL};
	t_shell data = {
		.env_list = NULL,
		.status = 42,
		.is_tty = true,
		.is_child = false
	};

	printf("\ncontext: exit + empty string: invalid numeric argument\n\n");
	printf("builtin_exit result:\n");
	int result = builtin_exit(tokens, &data);
	printf("bash behavior:\n");
	printf("exit\n");
	printf("bash: exit: : numeric argument required\n");
	printf(YEL "\nexpected return:\t-1 (exit signal)\n" RESET);
	printf(YEL "actual return:\t\t%d\n" RESET, result);
	printf(YEL "expected status:\t2\n" RESET);
	printf(YEL "actual status:\t\t%d\n" RESET, data.status);
	printf("RESULT: %s\n\n",
		(result == -1 && data.status == 2) ? GRN "PASS" RESET : RED "FAIL" RESET);
}

void test_exit_isolated_sign(void)
{
	printf(BR_BLU "TEST 9: exit with isolated sign - [exit +]\n" RESET);

	char *tokens[] = {"exit", "+", NULL};
	t_shell data = {
		.env_list = NULL,
		.status = 42,
		.is_tty = true,
		.is_child = false
	};

	printf("\ncontext: exit + isolated sign: '+' or '-' alone is invalid numeric argument\n\n");
	printf("builtin_exit result:\n");
	int result = builtin_exit(tokens, &data);
	printf("bash behavior:\n");
	printf("exit\n");
	printf("bash: exit: +: numeric argument required\n");
	printf(YEL "\nexpected return:\t-1 (exit signal)\n" RESET);
	printf(YEL "actual return:\t\t%d\n" RESET, result);
	printf(YEL "expected status:\t2\n" RESET);
	printf(YEL "actual status:\t\t%d\n" RESET, data.status);
	printf("RESULT: %s\n\n",
		(result == -1 && data.status == 2) ? GRN "PASS" RESET : RED "FAIL" RESET);
}

void test_exit_whitespace_number(void)
{
	printf(BR_BLU "TEST 10: exit with whitespace around number - [exit '  123  ']\n" RESET);

	char *tokens[] = {"exit", "  123  ", NULL};
	t_shell data = {
		.env_list = NULL,
		.status = 42,
		.is_tty = true,
		.is_child = false
	};

	printf("\ncontext: exit + whitespace: leading/trailing spaces should be trimmed by ft_atoll\n\n");
	printf("builtin_exit result:\n");
	int result = builtin_exit(tokens, &data);
	printf("bash behavior:\n");
	printf("exit\n");
	printf(YEL "\nexpected return:\t-1 (exit signal)\n" RESET);
	printf(YEL "actual return:\t\t%d\n" RESET, result);
	printf(YEL "expected status:\t123\n" RESET);
	printf(YEL "actual status:\t\t%d\n" RESET, data.status);
	printf("RESULT: %s\n\n",
		(result == -1 && data.status == 123) ? GRN "PASS" RESET : RED "FAIL" RESET);
}

void test_exit_number_with_suffix(void)
{
	printf(BR_BLU "TEST 11: exit with number followed by letters - [exit 42abc]\n" RESET);

	char *tokens[] = {"exit", "42abc", NULL};
	t_shell data = {
		.env_list = NULL,
		.status = 42,
		.is_tty = true,
		.is_child = false
	};

	printf("\ncontext: exit + trailing chars: number with suffix letters is invalid\n");
	printf("builtin_exit result:\n\n");
	int result = builtin_exit(tokens, &data);
	printf("bash behavior:\n");
	printf("exit\n");
	printf("bash: exit: 42abc: numeric argument required\n");
	printf(YEL "expected return:\t-1 (exit signal)\n" RESET);
	printf(YEL "actual return:\t\t%d\n" RESET, result);
	printf(YEL "expected status:\t2\n" RESET);
	printf(YEL "actual status:\t\t%d\n" RESET, data.status);
	printf("RESULT: %s\n\n",
		(result == -1 && data.status == 2) ? GRN "PASS" RESET : RED "FAIL" RESET);
}

int main(void)
{
	printf(MAG "=== EXIT BUILTIN TESTS (12 TESTS) ===" RESET "\n\n");

	test_exit_no_args();
	test_exit_valid_number();
	test_exit_too_many_args();
	test_exit_invalid_arg();
	test_exit_modulo_256();
	test_exit_negative_number();
	test_exit_overflow();
	test_exit_empty_string();
	test_exit_isolated_sign();
	test_exit_whitespace_number();
	test_exit_number_with_suffix();
	return (0);
}
