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
 * @brief Safely free all resources from a processed command line.
 *
 * Frees all dynamically allocated memory related to a parsed line,
 * including tokens, token list, AST, and the input line itself.
 * Each argument is checked for NULL and safely ignored if uninitialized.
 *
 * @param tokens     Array of token strings (can be NULL).
 * @param token_list Typed token list (can be NULL).
 * @param ast        Abstract syntax tree (can be NULL).
 * @param line       Input line buffer (can be NULL).
 */
void	cleanup_line(char **tokens,
		t_token *token_list, t_ast *ast, char *line)
{
	if (tokens)
	{
		free_strings_array(tokens);
		tokens = NULL;
	}
	if (token_list)
	{
		free_tokens_list(token_list);
		token_list = NULL;
	}
	if (ast)
	{
		free_ast(ast);
		ast = NULL;
	}
	if (line)
	{
		free(line);
		line = NULL;
	}
}
