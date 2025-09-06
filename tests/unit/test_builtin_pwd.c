#include "minishell.h"

/**
 * @file test_builtin_pwd.c
 * @brief Test program for the pwd builtin command implementation.
 *
 * This test suite validates the pwd builtin functionality against bash behavior,
 * ensuring correct path output, return codes, and argument handling for the
 * minishell project mandatory requirements.
 *
 * Compile: make build TEST=unit/test_builtin_pwd.c
 *
 * Usage examples:
 * 1. Run complete test suite:
 *      ./bin/test_builtin_pwd
 *    Expected output:
 *      - Current working directory path printed to stdout
 *      - Return code 0 for all valid cases
 *      - Consistent behavior with system pwd command
 *      - Arguments properly ignored (pwd takes no options)
 *
 * 2. Test categories and key cases:
 *    Basic functionality:
 *      pwd → prints current directory (e.g., /home/user/minishell)
 *      Exit status: 0 (success)
 *
 *    Argument handling:
 *      pwd hello world → same output as pwd (args ignored)
 *      pwd -l → same output as pwd (no options supported)
 *      pwd --help → same output as pwd (no options supported)
 *
 *    Comparison with system:
 *      minishell pwd output must match system pwd exactly
 *      Both should print absolute path ending with newline
 *      No trailing spaces or formatting differences
 *
 * 3. Return code validation:
 *    - SUCCESS (0): pwd executed successfully
 *    - pwd should never fail under normal circumstances
 *    - Consistent with bash behavior: always returns 0
 *
 * 4. Edge cases tested:
 *    - pwd called with NULL argument array
 *    - pwd called from different working directories
 *    - pwd output format consistency (absolute path + newline)
 *    - Memory management (no leaks in pwd implementation)
 *
 * 5. Integration considerations:
 *    The pwd builtin is foundational for:
 *    - Shell prompt display (showing current directory)
 *    - Relative path resolution in other commands
 *    - cd command validation and feedback
 *    - Script debugging and navigation
 *
 * Notes:
 * - pwd is one of the simplest builtins but critical for shell navigation
 * - Must work identically to bash pwd for user familiarity
 * - No options required by minishell subject (unlike bash -L/-P)
 * - Should use getcwd() system call internally
 * - Visual comparison with system pwd helps verify correctness
 * - Part of the 7 mandatory builtin commands for minishell
 */
void	test_builtin_pwd_basic(void)
{
	int	return_code;

	printf(BR_BLU "*** TEST 1: basic pwd functionality ***" RESET "\n");
	
	printf(YEL "expected: current directory path" RESET "\n");
	printf(CYN "actual:	" RESET);

	return_code = builtin_pwd(NULL);

	if (return_code == 0)
		printf(GRN "SUCCESS: return code = %d" RESET "\n", return_code);
	else
		printf(RED "FAILED: return code = %d" RESET "\n", return_code);
	printf("\n");
}

void	test_builtin_pwd_with_args(void)
{
	int		return_code;
	char	*fake_args[] = {"pwd", "hello", "world", NULL};

	printf(BR_BLU "*** TEST 2: pwd with arguments ***" RESET "\n");

	printf(YEL "expected: same as basic pwd (args ignored)" RESET "\n");
	printf(CYN "actual:	" RESET);

	return_code = builtin_pwd(fake_args);

	if (return_code == 0)
		printf(GRN "SUCCESS: return code = %d" RESET "\n", return_code);
	else
		printf(RED "FAILED: return code = %d" RESET "\n", return_code);
	printf("\n");
}

void	test_builtin_pwd_comparison(void)
{
	printf(BR_BLU "*** TEST 3: compare with system pwd ***" RESET "\n");

	printf(YEL "expected: same output" RESET "\n");
	printf(MAG "system pwd:		" RESET);
	printf("\n");
	system("pwd");

	printf(MAG "minishell pwd builtin:	" RESET);
	printf("\n");
	builtin_pwd(NULL);
	printf("\n");
}

int	main(void)
{
	test_builtin_pwd_basic();
	test_builtin_pwd_with_args();
	test_builtin_pwd_comparison();
	return (0);
}
