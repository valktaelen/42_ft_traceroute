/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aartiges <aartiges@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/16 00:33:50 by aartiges          #+#    #+#             */
/*   Updated: 2023/04/16 00:34:05 by aartiges         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/includes.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t			i;
	unsigned char	*bufsrc;
	unsigned char	*bufdst;

	if (!dst && !src)
		return (NULL);
	i = 0;
	bufsrc = (unsigned char *)src;
	bufdst = (unsigned char *)dst;
	while (i < n)
	{
		bufdst[i] = bufsrc[i];
		i++;
	}
	return (dst);
}

void	ft_bzero(void *s, size_t n)
{
	unsigned char	*array;
	size_t			index;

	array = s;
	index = 0;
	while (index < n)
	{
		array[index] = 0;
		index++;
	}
}

size_t	ft_strlen(const char *str)
{
	size_t	i;

	i = 0;
	while (str && str[i])
		++i;
	return (i);
}

char	*ft_strcpy(char *dst, const char *src)
{
	size_t	i;

	i = 0;
	while (src && src[i])
	{
		dst[i] = src[i];
		++i;
	}
	return (dst);
}

int	ft_strcmp(const char *str1, const char *str2)
{
	size_t	i;

	if (!str1 || !str2)
		return 1;
	i = 0;
	while (str1[i] && str2[i] && str1[i] == str2[i])
		++i;
	return (str1[i] - str2[i]);
}
