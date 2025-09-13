/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_safe_atoll.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pafroidu <pafroidu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 18:21:39 by diade-so          #+#    #+#             */
/*   Updated: 2025/09/13 15:53:36 by pafroidu         ###   ########.fr       */
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
 * @brief Check if adding a digit would cause overflow.
 * @param result Current accumulated result
 * @param digit Character digit to add ('0' to '9')
 * @param sign Sign of the number (1 for positive, -1 for negative)
 * @return 1 if safe to add, 0 if would overflow
 */
static int	check_overflow_before_add(long long result, char digit, int sign)
{
	if (sign == 1 && result > (LLONG_MAX - (digit - '0')) / 10)
		return (0);
	if (sign == -1 && (unsigned long long)result
		> ((unsigned long long)LLONG_MAX + 1 - (digit - '0')) / 10)
		return (0);
	return (1);
}

/**
 * @brief Safely converts a string to a long long.
 *
 * Allows leading and trailing whitespace, supports an optional '+' or '-'
 * and ensures the input is a valid long long within `LLONG_MIN` to `LLONG_MAX`.
 *
 * @param str Input string to convert.
 * @param out Pointer to store the result.
 * @return 1 on success, 0 on failure.
 */
int	ft_safe_atoll(const char *str, long long *out)
{
	long long	result;
	int			sign;

	if (!str || !out)
		return (0);
	str = skip_spaces(str);
	sign = parse_sign(&str);
	if (!ft_isdigit(*str))
		return (0);
	result = 0;
	while (ft_isdigit(*str))
	{
		if (!check_overflow_before_add(result, *str, sign))
			return (0);
		result = result * 10 + (*str - '0');
		str++;
	}
	str = skip_spaces(str);
	if (*str != '\0')
		return (0);
	*out = (result * sign);
	return (1);
}
