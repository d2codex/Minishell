#include "minishell.h"

/**
 * @file test_env.c
 * @brief Test program for init_env_from_envp() and environment list handling.
 *
 * This test initializes a linked list of t_env structs from the real environment
 * passed to the program, prints the contents, and frees memory. It demonstrates
 * correct parsing of "KEY=VALUE" strings and handling of empty or edge cases.
 *
 * Usage examples:
 * 1. Run with current shell environment:
 *      ./bin/test_env
 *    Expected output:
 *      - SUCCESS message
 *      - List of all environment variables in KEY=VALUE format
 *
 * 2. Run with custom variables using export:
 *      export NORMAL=VALUE
 *      export KEY_ONLY=
 *      export MULTI=EQUALS=TEST
 *      ./bin/test_env
 *    Expected output:
 *      - SUCCESS message
 *      - Prints the three variables as:
 *          NORMAL=VALUE
 *          KEY_ONLY=
 *          MULTI=EQUALS=TEST
 *
 * 3. Run with empty environment (using env -i):
 *      env -i ./bin/test_env
 *    Expected output:
 *      - FAILED message (returns NULL)
 *
 * Notes:
 * - Caller is responsible for freeing the environment list with ft_lstclear().
 * - This test prints colored output using RED/GRN/BLU macros from colors.h.
 */
/*
int	main(int argc, char **argv, char **envp)
{
	t_list	*env_list;

	(void)argc;
	(void)argv;
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
	return (0);
}
*/

/**
 * @file test_env_manual.c
 * @brief Manual environment array test for init_env_from_envp().
 *
 * This test initializes the environment list using a manually defined array
 * of environment strings, instead of relying on the shell's environment.
 * 
 * Usage:
 *    Comment out your other `main()` function that tests the real environment.
 */
int	main(void)
{
	t_list	*env_list;
	int		i;
	char	*manual_envp[] = {
		"KEY_ONLY=",
		"=VALUE_ONLY",
		"NORMAL=VALUE",
		"MULTI=EQUALS=TEST",
		NULL
	};

	env_list = init_env_from_envp(manual_envp);
	if (!env_list && errno == ENOMEM)
	{
		perror("init_env_from_envp");
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
	printf("\n%sExpected environment (manual test):%s\n", YEL, RESET);
	i = 0;
	while (manual_envp[i])
	{
		if (manual_envp[i][0] == '=')
			printf("%s%s (should be skipped / ignored)%s\n",
				RED, manual_envp[i], RESET);
		else
			printf("%s%s%s\n", GRN, manual_envp[i], RESET);
		i++;
	}
	ft_lstclear(&env_list, del_env);
	return (0);
}
