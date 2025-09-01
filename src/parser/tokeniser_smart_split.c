#include "minishell.h"

void	print_error(char *pre_msg, char *main_msg)
{
	if (pre_msg)
		ft_putstr_fd(pre_msg, STDERR_FILENO);
	if (main_msg)
		ft_putstr_fd(main_msg, STDERR_FILENO);
}

void	free_tab(char **tab, int count)
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

char	**split_shell_tokens(const char *s, char **tab)
{
	size_t	i;
	size_t	start;
	size_t	token;
	t_quote	current_quote_state;

	i = 0;
	token = 0;
	current_quote_state = STATE_NORMAL;
	while (s[i])
	{
		while (s[i] && is_a_shell_separator(current_quote_state, s[i]))
			i++;
		start = i;
		while (s[i] && !is_a_shell_separator(current_quote_state, s[i]))
		{
			current_quote_state = update_quote_state(current_quote_state, s[i]);
			i++;
		}
		if (i > start)
		{
			tab[token] = ft_substr(s, start, i - start);
			if (!tab[token])
			{
				free_tab(tab, token);
				return (NULL);
			}
			token++;
		}
	}
	tab[token] = NULL;
	return (tab);
}

char	**ft_split_tokens(char const *s)
{
	size_t	num_of_tokens;
	char	**final_tokens_tab;

	if (has_unclosed_quotes(s))
	{
		print_error("$Hell: ", "unclosed quote detected\n");
		return (NULL);
	}
	num_of_tokens = count_shell_tokens(s);
	final_tokens_tab = ft_calloc((num_of_tokens + 1), sizeof(char *));
	if (!final_tokens_tab)
		return (NULL);
	return (split_shell_tokens(s, final_tokens_tab));
}
