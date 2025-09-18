#include "minishell.h"

/**
 * @brief Print up to 4 message parts to stderr as one line
 *
 * @param p1 First part (required)
 * @param p2 Second part (can be NULL)
 * @param p3 Third part (can be NULL)
 * @param p4 Fourth part (can be NULL)
 */
void	print_error(char *p1, char *p2, char *p3, char *p4)
{
	if (p1)
		ft_putstr_fd(p1, STDERR_FILENO);
	if (p2)
		ft_putstr_fd(p2, STDERR_FILENO);
	if (p3)
		ft_putstr_fd(p3, STDERR_FILENO);
	if (p4)
		ft_putstr_fd(p4, STDERR_FILENO);
	ft_putchar_fd('\n', STDERR_FILENO);
}
