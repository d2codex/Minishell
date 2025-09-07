/**
 * @file test_env.c
 * @brief Test program for init_env_from_envp() and environment list handling.
 *
 * This test initializes a linked list of t_env structs from the real environment
 * passed to the program, prints the contents, and frees memory. It demonstrates
 * correct parsing of "KEY=VALUE" strings and handling of empty or edge cases.
 *
 * Compile: make build TEST=unit/test_env_from_envp.c
 *
 * Usage examples:
 * 1. Run with current shell environment:
 *      ./bin/test_env
 *    Expected output:
 *      - SUCCESS message
 *      - List of all environment variables in KEY=VALUE format
 *
 * 2. Run with custom variables using export:
 *    In the terminal, add these to the envp list (or create your own!):
 *      export NORMAL=VALUE
 *      export KEY_ONLY=
 *      export MULTI=EQUALS=TEST
 *      Compile and then run:
 *      ./bin/test_env
 *    Expected output:
 *      - SUCCESS message
 *      - Prints the three variables as:
 *          NORMAL=VALUE
 *          KEY_ONLY=
 *          MULTI=EQUALS=TEST
 *
 * 3. Run with empty environment (using env -i):
 *    Compile and run:
 *      env -i ./bin/test_env
 *    Expected output:
 *      - Environment is empty. Starting with no variables.
 *
 * 4. To comipile and run all tests(optional):
 *       make
 *
 * Notes:
 * - Caller is responsible for freeing the environment list with ft_lstclear().
 * - This test prints colored output using RED/GRN/BLU macros from colors.h.
 */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_list	*env_list;

	(void)argc;
	(void)argv;
	printf("%s[TEST] Importing real shell environment variables%s\n",
		BLU, RESET);
	env_list = init_env_from_envp(envp);
	if (!env_list && errno == ENOMEM)
	{
		printf("%sinit_env_from_envp%s\n", RED, RESET);
		perror("env_list");
		return (1);
	}
	if (!env_list)
	{
		printf("%sEnvironment is empty. Starting with no variables.%s\n",
			YEL, RESET);
		return (0);
	}
	printf("%sinit_env_from_envp: SUCCESS%s\n", GRN, RESET);
	printf("%sContents of env list:%s\n", BLU, RESET);
	print_env_list(env_list);
	ft_lstclear(&env_list, del_env);
	printf("===============================================================\n");
	return (0);
}
