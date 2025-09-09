#include "minishell.h"

/**
 * @file test_builtin_export.c
 * @brief Test program for builtin_export() and set_env_node().
 *
 * This test program simulates inserting and updating environment variables
 * in a t_env linked list using various valid and invalid identifiers. It
 * prints the key, value, in_env flag, and return status of set_env_node().
 *
 * The test covers:
 * - Creating new environment nodes from valid tokens with or without '='
 * - Updating existing nodes with new values
 * - Handling invalid identifiers (e.g., starting with digits or '=')
 * - Empty tokens
 * - Variables without values
 *
 * Compile:
 *      make build TEST=unit/test_builtin_export.c
 *
 * Usage examples:
 * 1. Run all tests:
 *      ./bin/test_builtin_export
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
int	main(void)
{
	int		i;
	int		size;
	t_list	*env_list;
	char	**tokens;
	t_env	**array;

	char *tokens_array[] = {
		"export",
		"FOO=bar",          // valid new variable
		"BAR=baz",          // valid new variable
		"EMPTY=",           // valid, empty value
		"UNSET",            // valid key, uninitialized value
		"FOO=newvalue",     // overwrite existing key
		"123INVALID=oops",  // invalid (starts with digit)
		"BAD-KEY=value",    // invalid (contains '-')
		"=novalue",         // invalid (no key before '=')
		"SP ACES=bad",      // invalid (contains space)
		"BAR=updated",      // overwrite existing key
		NULL
	};
	tokens = tokens_array;
	env_list = NULL;
	printf("====================EXPORT TEST====================\n");
	printf("Tokens:\n");
	i = 0;
	while (tokens[i])
	{
		printf("  [%d] %s\n", i, tokens[i]);
		i++;
	}
	printf("===================================================\n");
	if (builtin_export(&env_list, tokens) != 0)
	{
		printf("builtin_export failed\n");
		ft_lstclear(&env_list, del_env);
		return (1);
	}
	printf("\nSorted export list:\n");
	size = 0;
	array = export_list_to_array(env_list, &size);
	sort_export_array(array, size);
	print_sorted_export(array, size);
	free(array);
	printf("\nEnvironment list:\n");
	print_env_list(env_list);
	ft_lstclear(&env_list, del_env);
	printf("======================END TEST=====================\n");
	return (0);
}
