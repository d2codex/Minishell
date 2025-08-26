/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diade-so <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 15:11:48 by diade-so          #+#    #+#             */
/*   Updated: 2025/01/30 16:25:13 by diade-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include "libft.h"
# include <unistd.h>
# include <stdarg.h>
# include <stdint.h>

int	ft_printf(const char *str, ...);
int	process_conversion(va_list args, char c);
int	ft_putchar(char c);
int	ft_putstr(char *str);
int	ft_putnbr_signed(int nb);
int	ft_putnbr_unsigned(unsigned int nb);
int	decimal_to_hexa_uppercase(unsigned int nb);
int	decimal_to_hexa_lowercase(uintptr_t nb);
int	print_ptr_address(void *ptr);

#endif
