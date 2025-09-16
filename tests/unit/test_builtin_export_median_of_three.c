/**
 * @file test_median_of_three.c
 * @brief Manual test for median_of_three() function.
 *
 * This test initializes small arrays of t_env pointers with known
 * keys and checks that median_of_three() returns the index of the
 * median key among the low, mid, and high positions.
 *
 * Usage:
 * 1. Compile: make build TEST=unit/test_builtin_export_median_of_three.c
 *    Run: ./bin/test_builtin_export_median_of_three
 *
 * 2. To compile and run all tests (optional):
 *    make
 *
 * Notes:
 * - Prints original key order and the pivot index/key chosen.
 */
#include "minishell.h"

typedef struct t_median_test
{
	const char	*keys[5]; // max 5 keys for testing
	int			low;
	int			high;
}	t_median_test;

int	main(void)
{
	int		i;
	int		j;
	int		pivot_index;
	t_env	nodes[5];
	t_env	*array[5];

	t_median_test tests[] = {
	{{"A", "C", "B", NULL}, 0, 2},
	{{"X", "M", "Z", NULL}, 0, 2},
	{{"foo", "bar", "baz", NULL}, 0, 2},
	{{NULL, NULL, NULL}, 0, 0} // sentinel
	};
	printf("====================MEDIAN OF THREE TEST====================\n");
	i = 0;
	while (tests[i].keys[0])
	{
		j = 0;
		while (tests[i].keys[j])
		{
			nodes[j].key = (char *)tests[i].keys[j];
			array[j] = &nodes[j];
			j++;
		}
        pivot_index = median_of_three(array, tests[i].low, tests[i].high);
		printf(BLU "[%d]" RESET " Keys: ", i);
		j = 0;
		while (tests[i].keys[j])
		{
			printf("%s%s%s ", YEL, tests[i].keys[j], RESET);
			j++;
		}
		printf("\n  Pivot index: %d, Pivot key: %s%s%s\n\n",
			pivot_index, GRN, array[pivot_index]->key, RESET);
		i++;
	}
	printf("======================END TEST========================\n");
	return (0);
}
