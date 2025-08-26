/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atosize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diade-so <diade-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 19:33:53 by diade-so          #+#    #+#             */
/*   Updated: 2025/08/07 18:27:35 by diade-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stddef.h>
#include <stdint.h>

static const char	*skip_spaces(const char *str)
{
	while (*str && ft_isspace(*str))
		str++;
	return (str);
}

/**
 * @brief Safely converts a string to size_t.
 *
 * @details
 * Allows leading and trailing whitespaces, accepts an optional '+'.
 * Fails on overflow, invalid characters, or if the input is NULL,
 * empty, or just a '+'.
 *
 * @param str Input string to parse.
 * @param out Output pointer to store the result.
 * @return 1 on success, 0 on failure.
 */
int	ft_atosize(const char *str, size_t *out)
{
	size_t	result;

	if (!str || !out)
		return (0);
	str = skip_spaces(str);
	if (*str == '+')
		str++;
	if (!ft_isdigit(*str))
		return (0);
	result = 0;
	while (ft_isdigit(*str))
	{
		if (result > (SIZE_MAX - (*str - '0')) / 10)
			return (0);
		result = result * 10 + (*str - '0');
		str++;
	}
	str = skip_spaces(str);
	if (*str != '\0')
		return (0);
	*out = result;
	return (1);
}
