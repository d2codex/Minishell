#include "minishell.h"

int	main(void)
{
	printf(MAG "=== ECHO BUILTIN TESTS ===" RESET "\n\n");

	char	*tokens[] = {"echo", "hello", "world", "patate", NULL};
	char	*tokens1[] = {"echo", "-n", "hello", "world", "patate", NULL};
	char	*tokens2[] = {"echo", NULL};
	char	*tokens3[] = {"echo", "-n", NULL};
	t_shell	*data = {0};

	printf(YEL "notice, each test result is displayed between:\n" RESET);
	printf(YEL "------------------------\n" RESET);
	printf(YEL "------------------------\n" RESET);
	printf(YEL "in order to check if there is the newline or not\n" RESET);

	printf(BR_BLU "\nTEST 1: echo without -n option, non empty arguments list\n" RESET);
	printf("------------------------\n");
	builtin_echo(tokens, data);
	printf("------------------------\n\n");

	printf(BR_BLU "TEST 2: echo -n option, non empty arguments list\n" RESET);
	printf("------------------------\n");
	builtin_echo(tokens1, data);
	printf("------------------------\n\n");

	printf(BR_BLU "TEST 3: echo without arguments\n" RESET);
	printf("------------------------\n");
	builtin_echo(tokens2, data);
	printf("------------------------\n\n");

	printf(BR_BLU "TEST 4: echo -n option, without arguments\n" RESET);
	printf("------------------------\n");
	builtin_echo(tokens3, data);
	printf("------------------------\n\n");
	return (0);
}
