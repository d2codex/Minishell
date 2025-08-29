/*
	██╗    ██╗██╗██████╗
	██║    ██║██║██╔══██╗
	██║ █╗ ██║██║██████╔╝
	██║███╗██║██║██╔═══╝
	╚███╔███╔╝██║██║
	╚══╝╚══╝ ╚═╝╚═╝
*/

/* my own copy with indexs - still a very much regular and not smart split FOR NOW */

/*
	split s by delimiter c

	count_words :
	- skip c
	- count when s[i] != c && not in a word

	split_word :
	- skip c -> start = i
	- go until s[i] == c or end of string
	- if i > start we found a word -> ft_substr(s, start, i - start)
	(i == start = no word)

	- ft_substr(s, start, i - start) -> s[start to i-1]

	fail memory allocation case :
	- if ft_substr fails, word was already ++
	- free_tab(tab, word - 1)
*/

#include "libft.h"

static int	count_words(char const *s, char c)
{
	size_t	i;
	size_t	count;
	size_t	in_word;

	i = 0;
	count = 0;
	in_word = 0;
	while (s[i] != '\0')
	{
		if (s[i] != c && !in_word)
		{
			in_word = 1;
			count += 1;
		}
		else if (s[i] == c && in_word)
			in_word = 0;
		i++;
	}
	return (count);
}

static void	free_tab(char **tab, int count)
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

static char	**split_word(const char *s, char c, char **tab)
{
	size_t	i;
	size_t	start;
	size_t	word;

	i = 0;
	word = 0;
	while (s[i] != '\0')
	{
		while (s[i] != '\0' && s[i] == c)
			i++;
		start = i;
		while (s[i] != '\0' && s[i] != c)
			i++;
		if (i > start)
		{
			tab[word] = ft_substr(s, start, i - start);
			if (!tab[word++])
			{
				free_tab(tab, word - 1);
				return (NULL);
			}
		}
	}
	tab[word] = NULL;
	return (tab);
}

char	**ft_split(char const *s, char c)
{
	size_t	num_of_words;
	char	**tab;

	num_of_words = count_words(s, c);
	tab = ft_calloc((num_of_words + 1), sizeof(char *));
	if (!tab)
		return (NULL);
	return (split_word(s, c, tab));
}
