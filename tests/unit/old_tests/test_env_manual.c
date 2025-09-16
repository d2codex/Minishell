#include "minishell.h"

/**
 * @file test_env_manual.c
 * @brief Manual environment array test for init_env_from_envp().
 *
 * This test initializes the environment list using a manually defined array
 * of environment strings, instead of relying on the shell's environment.
 * 
 * Usage:
 * 1. Compile: make build TEST=unit/test_env_manual.c
 *    Run: ./bin/test_env_manual
 *    
 * 2. To compile and run all tests(optional):
 *    Compile: make
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

	printf("%s[TEST] Importing manually defined environment variables%s\n",
		BLU, RESET);
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
	printf("===============================================================\n");
	return (0);
}
