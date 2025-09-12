#include "minishell.h"

/**
* @brief Implements the pwd builtin command
*
* Prints the absolute pathname of the current working directory to stdout.
* Uses getcwd() system call to get the real current directory, ignoring
* the $PWD environment variable (like bash does).
*
* @param tokens Command arguments (unused, pwd takes no arguments)
* @param data Shell structure with infos (unused as well)
* @return EXIT_SUCCESS (0) on success, EXIT_FAILURE (1) on error
*
* @note Memory is automatically allocated by getcwd(NULL, 0) and freed
* @note Errors are handled with perror() which displays appropriate message
*/
int	builtin_pwd(char **tokens, t_shell *data)
{
	char	*path;

	(void)tokens;
	(void)data;
	path = getcwd(NULL, 0);
	if (!path)
	{
		perror("pwd");
		return (EXIT_FAILURE);
	}
	printf("%s\n", path);
	free(path);
	return (EXIT_SUCCESS);
}
