#include "minishell.h"

/**
 * @brief Free a dynamically allocated array of strings.
 *
 * Frees each string in the array up to the given count,
 * then frees the array itself.
 *
 * @param tab The array of strings to free
 * @param count Number of strings in the array
 */
void	free_string_array(char **tab, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}
