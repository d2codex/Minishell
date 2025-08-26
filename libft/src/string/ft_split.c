/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diade-so <diade-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 09:00:49 by diade-so          #+#    #+#             */
/*   Updated: 2025/04/26 23:12:24 by diade-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

static size_t	count_substr(const char *s, char c)
{
	size_t	count;
	int		in_substr;

	count = 0;
	in_substr = 0;
	if (!s || s[0] == '\0')
		return (0);
	while (*s)
	{
		if (*s != c && in_substr == 0)
		{
			count++;
			in_substr = 1;
		}
		else if (*s == c)
			in_substr = 0;
		s++;
	}
	return (count);
}

static char	*allocate_copy_substr(const char **s, char c )
{
	const char	*start;
	char		*substr;
	size_t		i;
	size_t		len;

	while (**s == c)
		(*s)++;
	if (**s == '\0')
		return (NULL);
	start = *s;
	len = 0;
	while ((*s)[len] && (*s)[len] != c)
		len++;
	substr = malloc((len + 1) * sizeof(char));
	if (!substr)
		return (NULL);
	i = 0;
	while (i < len)
	{
		substr[i] = start[i];
		i++;
	}
	substr[i] = '\0';
	*s += len;
	return (substr);
}

static void	free_split(char **array)
{
	size_t	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

char	**ft_split(char const *s, char c)
{
	size_t	i;
	size_t	count;
	char	**array;

	if (!s)
		return (NULL);
	count = count_substr(s, c);
	array = malloc((count + 1) * sizeof(*array));
	if (!array)
		return (NULL);
	i = 0;
	while (i < count)
	{
		array[i] = allocate_copy_substr(&s, c);
		if (!array[i])
		{
			free_split(array);
			return (NULL);
		}
		i++;
	}
	array[i] = NULL;
	return (array);
}
