/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diade-so <diade-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 19:58:43 by diade-so          #+#    #+#             */
/*   Updated: 2025/02/03 13:27:25 by diade-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	process_conversion(va_list args, char c)
{
	int	count;

	count = 0;
	if (c == 'c')
		count += ft_putchar(va_arg(args, int));
	else if (c == 's')
		count += ft_putstr(va_arg(args, char *));
	else if (c == 'p')
		count += print_ptr_address(va_arg(args, void *));
	else if (c == 'd' || c == 'i')
		count += ft_putnbr_signed(va_arg(args, int));
	else if (c == 'u')
		count += ft_putnbr_unsigned(va_arg(args, unsigned int));
	else if (c == 'x')
		count += decimal_to_hexa_lowercase((uintptr_t)va_arg(args,
					unsigned int));
	else if (c == 'X')
		count += decimal_to_hexa_uppercase(va_arg(args, unsigned int));
	else if (c == '%')
		count += ft_putchar('%');
	else
		return (-1);
	return (count);
}

int	ft_printf(const char *str, ...)
{
	int		i;
	int		count;
	va_list	args;

	if (!str)
		return (-1);
	i = 0;
	count = 0;
	va_start(args, str);
	while (str[i] != '\0')
	{
		if (str[i] == '%' && str[i + 1])
		{
			count += process_conversion(args, str[i + 1]);
			i++;
		}
		else
			count += ft_putchar(str[i]);
		i++;
	}
	va_end(args);
	return (count);
}
