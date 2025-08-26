/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diade-so <diade-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 14:40:54 by diade-so          #+#    #+#             */
/*   Updated: 2024/12/10 20:48:54 by diade-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <stdlib.h>
#include "libft.h"

static	int	count_digits(int n)
{
	int	count;

	count = 0;
	if (n <= 0)
		count = 1;
	if (n < 0)
		n *= -1;
	while (n != 0)
	{
		n /= 10;
		count++;
	}
	return (count);
}

char	*ft_itoa(int n)
{
	int		len;
	int		sign;
	char	*array;

	sign = 1;
	len = count_digits(n);
	if (n == 0)
		return (ft_strdup("0"));
	array = malloc(len + 1);
	if (!array)
		return (NULL);
	if (n < 0)
	{
		array[0] = '-';
		sign = -1;
	}
	array[len] = '\0';
	while (n != 0)
	{
		array[--len] = (n % 10) * sign + '0';
		n /= 10;
	}
	return (array);
}
