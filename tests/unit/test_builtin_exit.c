#include "minishell.h"

/*
 * @brief Implementation of the exit builtin command for minishell.
 *
 * Handles shell termination with proper exit code management
 * following bash behavior.
 * The function validates arguments and sets the exit status,
 * but delegates actual termination to the main shell loop via return codes.
 *
 * Ex of usage in execute_builtin()
 * in main function, check for is_tty and is_child, then :
 * int builtin_result = builtin_exit(tokens, shell_data);
 *
 * if (builtin_result == -1)
 *	{
 *		ft_lstclear(&shell_data->env_list, del_env);
 *		rl_clear_history();
 *		exit(shell_data->status);
 *	}
 *	if builtin_result == 0, continue normal execution
 *
 * @param tokens Command tokens array where tokens[0] is "exit"
 * @param data Shell data structure containing status and environment
 * @return -1 to signal main loop to exit, 0 to continue shell execution
 *
 * @note Exit codes are truncated to 8 bit range [0-255] like in bash
 * @note "Too many arguments" is the only case where shell doesn't exit
 *
 */
int	builtin_exit(char **tokens, t_shell *data)
{
	long long	exit_code;

	 if (data->is_tty && !data->is_child)
		printf("exit\n");
	if (tokens[1] == NULL)
	{
		if (data->is_tty && !data->is_child)
			printf("exit\n");
		return (-1);
	}
	if (tokens[2] != NULL)
	{
		printf("mini$Hell: exit: too many arguments\n");
		data->status = 1;
		return (0);
	}
	if (ft_safe_atoll(tokens[1], &exit_code) == 1)
	{
		data->status = (unsigned char)(exit_code);
		if (data->is_tty && !data->is_child)
			printf("exit\n");
		return (-1);
	}
	else
	{
		printf("mini$Hell: exit: %s: numeric argument required\n", tokens[1]);
		data->status = 2;
		return (-1);
	}
}

/*
 * Note: Tests intentionally uses separate functions for each test case
 * rather than a loop based approach. This design follows TDD (Test Driven Development)
 * methodology where each test represents a specific development iteration:
 *
 * 1. Write one failing test
 * 2. Implement minimal code to make it pass
 * 3. Refactor if needed
 * 4. Repeat with the next case
 *
 * each test function documents a step in the TDD progression.
 */void test_exit_no_args(void)
{
	printf(BR_BLU "TEST 1: exit without arguments\n" RESET);

	char *tokens[] = {"exit", NULL};
	t_shell data = {
		.env_list = NULL,
		.status = 42,
		.is_tty = true,
		.is_child = false
	};

	int result = builtin_exit(tokens, &data);
	printf("context: exit without argument signals exit with last status\n");
	printf(YEL "expected return:\t-1 (exit signal)\n" RESET);
	printf(YEL "actual return:\t\t%d\n" RESET, result);
	printf(YEL "expected status:\t42 (unchanged)\n" RESET);
	printf(YEL "actual status:\t\t%d\n" RESET, data.status);
	printf("RESULT: %s\n\n",
		(result == -1 && data.status == 42) ? GRN "PASS" RESET : RED "FAIL" RESET);
}

void test_exit_valid_number(void)
{
	printf(BR_BLU "TEST 2: exit with valid number\n" RESET);

	char *tokens[] = {"exit", "123", NULL};
	t_shell data = {
		.env_list = NULL,
		.status = 42,
		.is_tty = true,
		.is_child = false
	};

	int result = builtin_exit(tokens, &data);
	printf("context: exit with valid argument sets status and signals exit\n");
	printf(YEL "expected return:\t-1 (exit signal)\n" RESET);
	printf(YEL "actual return:\t\t%d\n" RESET, result);
	printf(YEL "expected status:\t123\n" RESET);
	printf(YEL "actual status:\t\t%d\n" RESET, data.status);
	printf("RESULT: %s\n\n",
		(result == -1 && data.status == 123) ? GRN "PASS" RESET : RED "FAIL" RESET);
}

void test_exit_too_many_args(void)
{
	printf(BR_BLU "TEST 3: exit with too many arguments\n" RESET);

	char *tokens[] = {"exit", "123", "456", "hello", NULL};
	t_shell data = {
		.env_list = NULL,
		.status = 42,
		.is_tty = true,
		.is_child = false
	};

	int result = builtin_exit(tokens, &data);
	printf("context: too many arguments sets error status, shell continues\n");
	printf(YEL "expected return:\t0 (continue shell)\n" RESET);
	printf(YEL "actual return:\t\t%d\n" RESET, result);
	printf(YEL "expected status:\t1\n" RESET);
	printf(YEL "actual status:\t\t%d\n" RESET, data.status);
	printf("RESULT: %s\n\n",
		(result == 0 && data.status == 1) ? GRN "PASS" RESET : RED "FAIL" RESET);
}

void test_exit_invalid_arg(void)
{
	printf(BR_BLU "TEST 4: exit with invalid argument\n" RESET);

	char *tokens[] = {"exit", "hello", NULL};
	t_shell data = {
		.env_list = NULL,
		.status = 42,
		.is_tty = true,
		.is_child = false
	};

	int result = builtin_exit(tokens, &data);
	printf("context: invalid argument shows error and signals exit\n");
	printf(YEL "expected return:\t-1 (exit signal)\n" RESET);
	printf(YEL "actual return:\t\t%d\n" RESET, result);
	printf(YEL "expected status:\t2\n" RESET);
	printf(YEL "actual status:\t\t%d\n" RESET, data.status);
	printf("RESULT: %s\n\n",
		(result == -1 && data.status == 2) ? GRN "PASS" RESET : RED "FAIL" RESET);
}

void test_exit_modulo_256(void)
{
	printf(BR_BLU "TEST 5: exit with number > 255\n" RESET);

	char *tokens[] = {"exit", "999", NULL};
	t_shell data = {
		.env_list = NULL,
		.status = 42,
		.is_tty = true,
		.is_child = false
	};

	int result = builtin_exit(tokens, &data);
	printf("context: 999 %% 256 = 231, modulo converts to valid range\n");
	printf(YEL "expected return:\t-1 (exit signal)\n" RESET);
	printf(YEL "actual return:\t\t%d\n" RESET, result);
	printf(YEL "expected status:\t231\n" RESET);
	printf(YEL "actual status:\t\t%d\n" RESET, data.status);
	printf("RESULT: %s\n\n",
		(result == -1 && data.status == 231) ? GRN "PASS" RESET : RED "FAIL" RESET);
}

void test_exit_negative_number(void)
{
	printf(BR_BLU "TEST 6: exit with negative number\n" RESET);

	char *tokens[] = {"exit", "-1", NULL};
	t_shell data = {
		.env_list = NULL,
		.status = 42,
		.is_tty = true,
		.is_child = false
	};

	int result = builtin_exit(tokens, &data);
	printf("context: -1 becomes 255 after modulo conversion (bash behavior)\n");
	printf(YEL "expected return:\t-1 (exit signal)\n" RESET);
	printf(YEL "actual return:\t\t%d\n" RESET, result);
	printf(YEL "expected status:\t255\n" RESET);
	printf(YEL "actual status:\t\t%d\n" RESET, data.status);
	printf("RESULT: %s\n\n",
		(result == -1 && data.status == 255) ? GRN "PASS" RESET : RED "FAIL" RESET);
}

void test_exit_overflow(void)
{
	printf(BR_BLU "TEST 7: exit with overflow number\n" RESET);

	char *tokens[] = {"exit", "999999999999999999999999", NULL};
	t_shell data = {
		.env_list = NULL,
		.status = 42,
		.is_tty = true,
		.is_child = false
	};

	int result = builtin_exit(tokens, &data);
	printf("context: number exceeds LONG_MAX, treated as invalid\n");
	printf(YEL "expected return:\t-1 (exit signal)\n" RESET);
	printf(YEL "actual return:\t\t%d\n" RESET, result);
	printf(YEL "expected status:\t2\n" RESET);
	printf(YEL "actual status:\t\t%d\n" RESET, data.status);
	printf("RESULT: %s\n\n",
		(result == -1 && data.status == 2) ? GRN "PASS" RESET : RED "FAIL" RESET);
}

void test_exit_empty_string(void)
{
	printf(BR_BLU "TEST 8: exit with empty string\n" RESET);

	char *tokens[] = {"exit", "", NULL};
	t_shell data = {
		.env_list = NULL,
		.status = 42,
		.is_tty = true,
		.is_child = false
	};

	int result = builtin_exit(tokens, &data);
	printf("context: empty string is invalid numeric argument\n");
	printf(YEL "expected return:\t-1 (exit signal)\n" RESET);
	printf(YEL "actual return:\t\t%d\n" RESET, result);
	printf(YEL "expected status:\t2\n" RESET);
	printf(YEL "actual status:\t\t%d\n" RESET, data.status);
	printf("RESULT: %s\n\n",
		(result == -1 && data.status == 2) ? GRN "PASS" RESET : RED "FAIL" RESET);
}

void test_exit_isolated_sign(void)
{
	printf(BR_BLU "TEST 9: exit with isolated sign\n" RESET);

	char *tokens[] = {"exit", "+", NULL};
	t_shell data = {
		.env_list = NULL,
		.status = 42,
		.is_tty = true,
		.is_child = false
	};

	int result = builtin_exit(tokens, &data);
	printf("context: isolated '+' or '-' sign is invalid\n");
	printf(YEL "expected return:\t-1 (exit signal)\n" RESET);
	printf(YEL "actual return:\t\t%d\n" RESET, result);
	printf(YEL "expected status:\t2\n" RESET);
	printf(YEL "actual status:\t\t%d\n" RESET, data.status);
	printf("RESULT: %s\n\n",
		(result == -1 && data.status == 2) ? GRN "PASS" RESET : RED "FAIL" RESET);
}

int main(void)
{
	printf(MAG "=== EXIT BUILTIN TESTS ===" RESET "\n\n");

	test_exit_no_args();
	test_exit_valid_number();
	test_exit_too_many_args();
	test_exit_invalid_arg();
	test_exit_modulo_256();
	test_exit_negative_number();
	test_exit_overflow();
	test_exit_empty_string();
	test_exit_isolated_sign();

	return (0);
}
