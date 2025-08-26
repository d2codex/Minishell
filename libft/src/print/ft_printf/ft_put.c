/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_put.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diade-so <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 14:25:10 by diade-so          #+#    #+#             */
/*   Updated: 2025/01/30 16:32:19 by diade-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_putchar(char c)
{
	write (1, &c, 1);
	return (1);
}

int	ft_putstr(char *str)
{
	int	i;

	if (str == NULL)
	{
		write(1, "(null)", 6);
		return (6);
	}
	i = 0;
	while (str[i])
	{
		ft_putchar(str[i]);
		i++;
	}
	return (i);
}

int	ft_putnbr_signed(int nb)
{
	int	count;
	int	sign;

	count = 0;
	sign = 1;
	if (nb < 0)
	{
		count += ft_putchar('-');
		sign = -1;
	}
	if (nb / 10)
	{
		count += ft_putnbr_signed((nb / 10) * sign);
		count += ft_putchar('0' + nb % 10 * sign);
	}
	else
		count += ft_putchar('0' + nb * sign);
	return (count);
}

int	ft_putnbr_unsigned(unsigned int nb)
{
	int	count;

	count = 0;
	if (nb > 9)
	{
		count += ft_putnbr_unsigned(nb / 10);
		count += ft_putchar('0' + nb % 10);
	}
	else
		count += ft_putchar('0' + nb);
	return (count);
}
