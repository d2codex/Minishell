#include "minishell.h"

void	pwd_tests(void)
{
	int return_code;

	return_code = print_working_directory(NULL);
	printf("DEBUG: function returned: [%d] code\n", return_code);
}
int	main(void)
{
	pwd_tests();
	return (0);
}
