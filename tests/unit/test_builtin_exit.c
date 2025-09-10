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
	 // exit with one valid argument, return code (mod 256 converts to range [0-255])
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
	// exit with too many arguments won't exit the shell, returns error code 1
	printf("mini$Hell: exit: too many arguments\n");
	return (1);
}
/*
 * Note: This test suite intentionally uses separate functions for each test case
 * rather than a loop based approach. This design follows TDD (Test Driven Development)
 * methodology where each test represents a specific development iteration:
 *
 * 1. Write one failing test
 * 2. Implement minimal code to make it pass
 * 3. Refactor if needed
 * 4. Repeat with the next case
 *
 * Each test function documents a step in the TDD progression.
 */
void test_exit_no_args(void)
{
	printf(BR_BLU "TEST 1: exit without arguments\n" RESET);

	char *tokens[] = {"exit", NULL};
	t_shell data = {
		.env_list = NULL,
		.last_exit_code = 42,
		.is_interactive_mode = true,
		.is_child = false
	};

	int result = builtin_exit(tokens, &data);
	printf("context: exit without argument returns the last known exit code\n");
	printf(YEL "expected:\tcode 42\n" RESET);
	printf(YEL "result:\t\tcode %d\n" RESET, result);
	printf("RESULT: %s\n\n", result == 42 ? GRN "PASS" RESET : RED "FAIL" RESET);
}

void test_exit_valid_number(void)
{
	printf(BR_BLU "TEST 2: exit with valid number\n" RESET);

	char *tokens[] = {"exit", "123", NULL};
	t_shell data = {
		.env_list = NULL,
		.last_exit_code = 42,
		.is_interactive_mode = true,
		.is_child = false
	};

	int result = builtin_exit(tokens, &data);
	printf("context: exit with valid argument returns the argument value\n");
	printf(YEL "expected:\tcode 123\n" RESET);
	printf(YEL "result:\t\tcode %d\n" RESET, result);
	printf("RESULT: %s\n\n", result == 123 ? GRN "PASS" RESET : RED "FAIL" RESET);
}

void test_exit_too_many_args(void)
{
	printf(BR_BLU "TEST 3: exit with too many arguments\n" RESET);

	char *tokens[] = {"exit", "123", "456", "hello", NULL};
	t_shell data = {
		.env_list = NULL,
		.last_exit_code = 42,
		.is_interactive_mode = true,
		.is_child = false
	};

	int result = builtin_exit(tokens, &data);
	printf("context: too many arguments returns error code 1, shell stays open\n");
	printf(YEL "expected:\tcode 1\n" RESET);
	printf(YEL "result:\t\tcode %d\n" RESET, result);
	printf("RESULT: %s\n\n", result == 1 ? GRN "PASS" RESET : RED "FAIL" RESET);
}

void test_exit_invalid_arg(void)
{
	printf(BR_BLU "TEST 4: exit with invalid argument\n" RESET);

	char *tokens[] = {"exit", "hello", NULL};
	t_shell data = {
		.env_list = NULL,
		.last_exit_code = 42,
		.is_interactive_mode = true,
		.is_child = false
	};

	int result = builtin_exit(tokens, &data);
	printf("context: invalid argument shows error message and returns code 2\n");
	printf(YEL "expected:\tcode 2\n" RESET);
	printf(YEL "result:\t\tcode %d\n" RESET, result);
	printf("RESULT: %s\n\n", result == 2 ? GRN "PASS" RESET : RED "FAIL" RESET);
}

void test_exit_modulo_256(void)
{
	printf(BR_BLU "TEST 5: exit with number > 255\n" RESET);

	char *tokens[] = {"exit", "999", NULL};
	t_shell data = {
		.env_list = NULL,
		.last_exit_code = 42,
		.is_interactive_mode = true,
		.is_child = false
	};

	int result = builtin_exit(tokens, &data);
	printf("context: 999 %% 256 = 231, modulo converts to valid range\n");
	printf(YEL "expected:\tcode 231\n" RESET);
	printf(YEL "result:\t\tcode %d\n" RESET, result);
	printf("RESULT: %s\n\n", result == 231 ? GRN "PASS" RESET : RED "FAIL" RESET);
}

void test_exit_negative_number(void)
{
	printf(BR_BLU "TEST 6: exit with negative number\n" RESET);

	char *tokens[] = {"exit", "-1", NULL};
	t_shell data = {
		.env_list = NULL,
		.last_exit_code = 42,
		.is_interactive_mode = true,
		.is_child = false
	};

	int result = builtin_exit(tokens, &data);
	printf("context: -1 becomes 255 after modulo conversion (bash behavior)\n");
	printf(YEL "expected:\tcode 255\n" RESET);
	printf(YEL "result:\t\tcode %d\n" RESET, result);
	printf("RESULT: %s\n\n", result == 255 ? GRN "PASS" RESET : RED "FAIL" RESET);
}

void test_exit_overflow(void)
{
	printf(BR_BLU "TEST 7: exit with overflow number\n" RESET);

	char *tokens[] = {"exit", "999999999999999999999999", NULL};
	t_shell data = {
		.env_list = NULL,
		.last_exit_code = 42,
		.is_interactive_mode = true,
		.is_child = false
	};

	int result = builtin_exit(tokens, &data);
	printf("context: number exceeds LONG_MAX, treated as invalid\n");
	printf(YEL "expected:\tcode 2\n" RESET);
	printf(YEL "result:\t\tcode %d\n" RESET, result);
	printf("RESULT: %s\n\n", result == 2 ? GRN "PASS" RESET : RED "FAIL" RESET);
}

void test_exit_empty_string(void)
{
	printf(BR_BLU "TEST 8: exit with empty string\n" RESET);

	char *tokens[] = {"exit", "", NULL};
	t_shell data = {
		.env_list = NULL,
		.last_exit_code = 42,
		.is_interactive_mode = true,
		.is_child = false
	};

	int result = builtin_exit(tokens, &data);
	printf("context: empty string is invalid numeric argument\n");
	printf(YEL "expected:\tcode 2\n" RESET);
	printf(YEL "result:\t\tcode %d\n" RESET, result);
	printf("RESULT: %s\n\n", result == 2 ? GRN "PASS" RESET : RED "FAIL" RESET);
}

void test_exit_isolated_sign(void)
{
	printf(BR_BLU "TEST 9: exit with isolated sign\n" RESET);

	char *tokens[] = {"exit", "+", NULL};
	t_shell data = {
		.env_list = NULL,
		.last_exit_code = 42,
		.is_interactive_mode = true,
		.is_child = false
	};

	int result = builtin_exit(tokens, &data);
	printf("context: isolated '+' or '-' sign is invalid\n");
	printf(YEL "expected:\tcode 2\n" RESET);
	printf(YEL "result:\t\tcode %d\n" RESET, result);
	printf("RESULT: %s\n\n", result == 2 ? GRN "PASS" RESET : RED "FAIL" RESET);
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
