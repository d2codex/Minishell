#include "minishell_tests.h"
#include "minishell.h"

void	pwd_tests(void)
{
	int return_code;
	
	return_code = print_working_directory(NULL);  
	printf("DEBUG: function returned: [%d] code\n", return_code);
}