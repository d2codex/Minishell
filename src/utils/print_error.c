#include "minishell.h"

/**
 * @brief Print an error message to standard error.
 *
 * Prints an optional prefix message and a main message to STDERR.
 *
 * @param pre_msg Optional prefix message (can be NULL)
 * @param main_msg Main message to print (can be NULL)
 */
void	print_error(char *pre_msg, char *main_msg)
{
	if (pre_msg)
		ft_putstr_fd(pre_msg, STDERR_FILENO);
	if (main_msg)
		ft_putstr_fd(main_msg, STDERR_FILENO);
	ft_putchar_fd('\n', STDERR_FILENO);
}
