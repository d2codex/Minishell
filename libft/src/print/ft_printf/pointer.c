/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pointer.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diade-so <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 14:01:12 by diade-so          #+#    #+#             */
/*   Updated: 2025/01/31 10:02:59 by diade-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	decimal_to_hexa_uppercase(unsigned int nb)
{
	int		count;
	char	*base_16;

	count = 0;
	base_16 = "0123456789ABCDEF";
	if (nb >= 16)
		count += decimal_to_hexa_uppercase(nb / 16);
	count += ft_putchar(base_16[nb % 16]);
	return (count);
}

int	decimal_to_hexa_lowercase(uintptr_t nb)
{
	int		count;
	char	*base_16;

	count = 0;
	base_16 = "0123456789abcdef";
	if (nb >= 16)
		count += decimal_to_hexa_lowercase(nb / 16);
	count += ft_putchar(base_16[nb % 16]);
	return (count);
}

int	print_ptr_address(void *ptr)
{
	int			count;
	uintptr_t	address;

	address = (uintptr_t)ptr;
	count = 0;
	if (ptr == NULL)
		return (ft_putstr("(nil)"));
	count += ft_putstr("0x");
	count += decimal_to_hexa_lowercase(address);
	return (count);
}
