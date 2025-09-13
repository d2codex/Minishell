#include "minishell.h"

/**
 * @brief Print up to 4 message parts to stderr as one line
 *
 * @param part1 First part (required)
 * @param part2 Second part (can be NULL)
 * @param part3 Third part (can be NULL)
 * @param part4 Fourth part (can be NULL)
 */
void	print_error_multi(char *part1, char *part2, char *part3, char *part4)
{
	if (part1)
		ft_putstr_fd(part1, STDERR_FILENO);
	if (part2)
		ft_putstr_fd(part2, STDERR_FILENO);
	if (part3)
		ft_putstr_fd(part3, STDERR_FILENO);
	if (part4)
		ft_putstr_fd(part4, STDERR_FILENO);
	ft_putchar_fd('\n', STDERR_FILENO);
}
