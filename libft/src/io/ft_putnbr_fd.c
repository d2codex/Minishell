/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diade-so <diade-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 22:19:25 by diade-so          #+#    #+#             */
/*   Updated: 2024/12/10 20:03:45 by diade-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h" 
#include <unistd.h>

void	ft_putnbr_fd(int n, int fd)
{
	int	sign;

	sign = 1;
	if (n < 0)
	{
		ft_putchar_fd('-', fd);
		sign = -1;
	}
	if (n >= 10 || n <= -10)
	{
		ft_putnbr_fd(((n / 10) * sign), fd);
		ft_putchar_fd('0' + n % 10 * sign, fd);
	}
	else
		ft_putchar_fd('0' + n * sign, fd);
}
