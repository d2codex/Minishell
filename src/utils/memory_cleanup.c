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
void	free_string_array(char **tab, size_t count)
{
	size_t	i;

	if (!tab)
		return ;
	i = 0;
	while (i < count)
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

/**
 * @brief Clean up shell resources before exit
 *
 * @param data Shell data structure to clean
 */
void	cleanup_shell(t_shell *data)
{
	if (!data)
		return ;
	if (data->env_list)
		ft_lstclear(&data->env_list, del_env);
	rl_clear_history();
}

/**
 * @brief Clean up process_line resources
 *
 * @param tokens Token array to free (can be NULL)
 * @param line Input line to free (can be NULL)
 */
void	cleanup_process_line(char **tokens, char *line)
{
	int	token_count;

	if (tokens)
	{
		token_count = 0;
		while (tokens[token_count])
			token_count++;
		free_string_array(tokens, token_count);
	}
	if (line)
		free(line);
}
