/**
 * @file test_swap_env.c
 * @brief Manual test for swap_env() function.
 *
 * This test verifies the swap_env() function by swapping pairs of t_env
 * nodes and printing the results in a clear before/after format.
 *
 * Usage:
 * 1. Compile: make build TEST=unit/test_swap_env.c
 *    Run: ./bin/test_swap_env
 *    
 * 2. To compile and run all tests (optional):
 *    Compile: make
 */
#include "minishell.h"

t_env	*make_env(const char *key)
{
	t_env	*e;

	e = malloc(sizeof(t_env));
	if (!e)
		return (NULL);
	e->key = strdup(key);
	e->value = NULL;
	return (e);
}

typedef struct s_swap_test
{
	const char	*key_a;
	const char	*key_b;
}	t_swap_test;

int	main(void)
{
	int		i;
	t_env	*a;
	t_env	*b;
	t_env	*pa;
	t_env	*pb;

	t_swap_test tests[] = {
	{"A", "B"},
	{"Hello", "World"},
	{"Same", "Same"},
	{NULL, NULL} // sentinel
	};
	printf("======================== SWAP_ENV TEST ========================\n");
	i = 0;
	while (tests[i].key_a && tests[i].key_b)
	{
		a = make_env(tests[i].key_a);
		b = make_env(tests[i].key_b);
		pa = a;
		pb = b;
		printf(BLU "[%d] Test swapping \"%s\" <-> \"%s\"\n" RESET,
			i, a->key, b->key);
		printf(YEL "Before: " RESET "a = %s, b = %s\n", pa->key, pb->key);
		swap_env(&pa, &pb);
		printf(YEL "After : " RESET "a = %s, b = %s\n", pa->key, pb->key);
		if ((strcmp(pa->key, tests[i].key_b) == 0
				&& strcmp(pb->key, tests[i].key_a) == 0))
			printf(GRN "Result : SUCCESS\n\n" RESET);
		else
			printf(RED "Result : FAIL\n\n" RESET);
		free(a->key);
		free(b->key);
		free(a);
		free(b);
		i++;
	}
	printf("========================== END TEST ==========================\n");
	return (0);
}
