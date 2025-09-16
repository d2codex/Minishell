/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diade-so <diade-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 10:18:06 by diade-so          #+#    #+#             */
/*   Updated: 2025/09/09 10:22:34 by diade-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief Compare two null-terminated strings (env keys).
 *
 * Works like strcmp: returns 0 if equal, a negative value if s1 < s2,
 * and a positive value if s1 > s2 (lexicographically).
 *
 * @param s1 First string to compare.
 * @param s2 Second string to compare.
 * @return int Comparison result.
 */
int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && *s2)
	{
		if (*s1 != *s2)
			return ((unsigned char)*s1 - (unsigned char)*s2);
		s1++;
		s2++;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}
