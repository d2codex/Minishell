/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_safe_atol.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pafroidu <pafroidu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 18:21:39 by diade-so          #+#    #+#             */
/*   Updated: 2025/09/10 16:05:03 by pafroidu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
//#include <ctype.h>
#include <limits.h>

static const char	*skip_spaces(const char *str)
{
	while (*str && ft_isspace(*str))
		str++;
	return (str);
}

static int	parse_sign(const char **str)
{
	int	sign;

	sign = 1;
	if (**str == '-' || **str == '+')
	{
		if (**str == '-')
			sign = -1;
		(*str)++;
	}
	return (sign);
}

/**
 * @brief Safely converts a string to a long.
 *
 * Allows leading and trailing whitespace, supports an optional '+' or '-'
 * and ensures the input is a valid long within `LONG_MIN` to `LONG_MAX`.
 *
 * @param str Input string to convert.
 * @param out Pointer to store the result.
 * @return 1 on success, 0 on failure.
 */
int	ft_safe_atol(const char *str, long *out)
{
	long	result;
	int		sign;

	if (!str || !out)
		return (0);
	str = skip_spaces(str);
	sign = parse_sign(&str);
	if (!ft_isdigit(*str))
		return (0);
	result = 0;
	while (ft_isdigit(*str))
	{
		result = result * 10 + (*str - '0');
		if ((sign == 1 && result > LONG_MAX)
			|| (sign == -1
				&& (unsigned long)result > (unsigned long)LONG_MAX + 1))
			return (0);
		str++;
	}
	str = skip_spaces(str);
	if (*str != '\0')
		return (0);
	*out = (result * sign);
	return (1);
}
