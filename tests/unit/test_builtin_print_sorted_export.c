/**
 * @file test_builtin_print_sorted_export.c
 * @brief Test program for printing sorted environment variables with export format.
 *
 * This test program simulates creating a list of environment variables
 * using set_env_node(), converting it to an array, sorting it, and printing
 * it in the format used by the shell's export builtin.
 *
 * The test covers:
 * - Creating environment variables with valid keys (with and without values)
 * - Converting the linked list to an array
 * - Sorting with sort_export_array() (insertion sort / quicksort)
 * - Printing the sorted list with print_sorted_export()
 *
 * Compile:
 *      make build TEST=unit/test_builtin_print_sorted_export.c
 *
 * Usage examples:
 * 1. Run the test:
 *      ./bin/test_builtin_print_sorted_export
 *    Expected output:
 *      - Sorted list of environment variables in "declare -x KEY="VALUE"" format
 *      - Variables without values printed as "declare -x KEY"
 *      - Output is sorted alphabetically by key
 *
 * Notes:
 * - The environment list is initially empty and populated from the tokens array.
 * - Caller is responsible for freeing the list using ft_lstclear().
 * - This test demonstrates proper handling of keys with empty values and overwrites.
 */
#include "minishell.h"

int	main(void)
{
	int		i;
	int		size;
	t_env	**array;
	t_list	*env_list;

	char	*tokens[] = {
		"FOO=bar",        "BAR",             "_UNDERSCORE=ok", "BAZ=",
		"ALPHA=1",        "BETA=2",          "GAMMA=3",        "DELTA=4",
		"EPSILON=5",      "ZETA=6",          "ETA=7",          "THETA=8",
		"IOTA=9",         "KAPPA=10",        "LAMBDA=11",      "MU=12",
		"NU=13",          "XI=14",           "OMICRON=15",     "PI=16",
		"RHO=17",         "SIGMA=18",        "TAU=19",         "UPSILON=20",
		"PHI=21",         "CHI=22",          "PSI=23",         "OMEGA=24",
		"ONE",            "TWO",             "THREE",          "FOUR",
		"FIVE",           "SIX",             "SEVEN",          "EIGHT",
		"NINE",           "TEN",             "HELLO_WORLD=ok", "GOODBYE=bye",
		"_AAA",           "_aaa",            "_Abc",           "_abc",
		NULL
	};

	env_list = NULL;
	size = 0;
	i = 0;
	printf("====================EXPORT PRINT TEST====================\n");
	while (tokens[i])
	{
		if (!set_env_node(&env_list, tokens[i]))
		{
			printf("Failed to set env node for token: %s\n", tokens[i]);
			ft_lstclear(&env_list, del_env);
			return (1);
		}
		i++;
	}
	array = export_list_to_array(env_list, &size);
	sort_export_array(array, size);
	if (!array)
	{
		printf("No environment variables to print.\n");
		ft_lstclear(&env_list, del_env);
		return (0);
	}
	print_sorted_export(array, size);
	free(array);
	ft_lstclear(&env_list, del_env);
	printf("======================END TEST=========================\n");
	return (0);
}
