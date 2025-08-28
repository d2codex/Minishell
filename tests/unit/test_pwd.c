#include "minishell.h"
#include "libft.h"

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
