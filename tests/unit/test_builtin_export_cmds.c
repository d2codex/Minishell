/**
 * @file test_builtin_export_cmds.c
 * @brief Test program for set_env_node() and export handling.
 *
 * This test program simulates inserting and updating environment variables
 * in a t_env linked list, using various valid and invalid identifiers. It
 * prints the key, value, in_env flag, old values (if overwritten), and the
 * return status of set_env_node().
 *
 * The test covers:
 * - Creating new environment nodes from valid tokens with or without '='
 * - Updating existing nodes with new values
 * - Handling invalid identifiers (e.g., starting with digits or '=')
 * - Empty tokens
 * - Variables without values
 *
 * Compile:
 *      make build TEST=unit/test_builtin_export_cmds.c
 *
 * Usage examples:
 * 1. Run all tests:
 *      ./bin/test_builtin_export_cmds
 *    Expected output:
 *      - Each test prints:
 *          - Input token
 *          - Return value (0=failure, 1=success)
 *          - Old key/value/in_env if node existed
 *          - Current key/value/in_env
 *          - Expected in_env
 *
 * 2. Invalid identifiers:
 *      Tokens like "123VAL=NUM", "=no_key", or "" are tested to ensure
 *      set_env_node() rejects them and prints an error message.
 *
 * Notes:
 * - The environment list is initially empty and grows dynamically.
 * - Caller is responsible for freeing the list using ft_lstclear().
 * - Boolean flags are printed as raw 0/1 values.
 */

#include "minishell.h"

typedef struct s_env_test
{
	const char	*token;
	bool		expected_in_env;
}	t_env_test;

int	main(void)
{
	int			i;
	t_list		*env_list;
	t_env		*node;
	t_env		*existing_node;
	char		*key;
	int			return_value;

	t_env_test	tests[] = {
	{"FOO=bar", true},
	{"_BAR", false},
	{"_BAR___", false},
	{"_BAR _ ", false},
	{"BAR=baz", true},
	{"BAZ=", true},
	{"NO_VALUE", false},
	{"123VAL=NUM", false},
	{"", false}, // empty key
	{"=no_key", false},
	{"BAZ=new_value", true},
	{"BAZ=boo", true},
		// add more tests here
	{NULL, 0} // sentinel
	};

	printf("======================EXPORT AND ARGS TEST=====================\n");
	env_list = NULL;
	i = 0;
	while (tests[i].token != NULL)
	{
		printf(BLU "[%02d]" RESET " Input: " YEL "%s" RESET "\n", i, tests[i].token);
		// extract key from token
		key = get_env_key(tests[i].token);
		existing_node = NULL;
		if (key)
			existing_node = get_env_node_by_key(env_list, key);
		if (existing_node)
		{
			printf("   old key: %s%s%s\n", MAG, existing_node->key, RESET);
			if (existing_node->value)
				printf(" old value: %s%s%s\n", CYN, existing_node->value, RESET);
			else
				printf(" old value: %sNULL%s\n", CYN, RESET);
			printf("old in_env: %s%d%s\n", BLU, existing_node->in_env, RESET);
		}
		return_value = set_env_node(&env_list, tests[i].token);
		printf("    return: %d\n", return_value);

		/* Look up the node after insertion/update */
		if (key)
		{
			node = get_env_node_by_key(env_list, key);
			free(key);
		}
		else
		{
			printf("\n");
			node = NULL;
		}
		/* Print current node info */
		if (node)
		{
			printf("       key: %s%s%s\n", MAG, node->key, RESET);
			if (node->value)
				printf(" new value: %s%s%s\n", CYN, node->value, RESET);
			else
				printf(" new value: %sNULL%s\n", CYN, RESET);
			printf("    in_env: %s%d%s\n", BLU, node->in_env, RESET);
			printf("  expected: %s%d%s\n\n", GRN, tests[i].expected_in_env, RESET);
		}
		i++;
	}
	ft_lstclear(&env_list, del_env);
	printf("============================END TEST==========================\n");
	return (0);
}
