/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diade-so <diade-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 16:34:19 by diade-so          #+#    #+#             */
/*   Updated: 2024/12/09 12:49:18 by diade-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	int		i;
	int		len;
	char	*array;

	len = ft_strlen(s);
	array = malloc(len + 1);
	if (!array)
		return (NULL);
	i = 0;
	while (i < len)
	{
		array[i] = f(i, s[i]);
		i++;
	}
	array [i] = '\0';
	return (array);
}
