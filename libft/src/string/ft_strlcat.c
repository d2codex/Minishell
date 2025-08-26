/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diade-so <diade-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 15:37:04 by diade-so          #+#    #+#             */
/*   Updated: 2025/05/03 12:26:21 by diade-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include "libft.h"
#include <string.h>

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	size_t	i;
	size_t	d_len;
	size_t	s_len;

	i = 0;
	d_len = strlen(dest);
	s_len = strlen(src);
	if (d_len >= size)
		return (size + s_len);
	if (d_len < size - 1 && size > 0)
	{
		while (src[i] && i < size - d_len - 1)
		{
			dest[d_len + i] = src[i];
			i++;
		}
		dest[d_len + i] = '\0';
	}
	return (d_len + s_len);
}
