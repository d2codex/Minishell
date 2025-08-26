/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diade-so <diade-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 23:15:04 by diade-so          #+#    #+#             */
/*   Updated: 2025/04/17 19:59:48 by diade-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

void	*ft_calloc(size_t nmemb, size_t size)
{
	size_t			i;
	unsigned char	*arr;

	if (nmemb == 0 || size == 0)
		return (NULL);
	if (size > SIZE_MAX / nmemb)
		return (NULL);
	arr = (unsigned char *)malloc(nmemb * size);
	if (!arr)
		return (NULL);
	i = 0;
	while (i < nmemb * size)
	{
		arr[i] = 0;
		i++;
	}
	return (arr);
}
