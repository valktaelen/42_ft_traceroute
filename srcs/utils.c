/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aartiges <aartiges@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/15 01:46:05 by aartiges          #+#    #+#             */
/*   Updated: 2023/04/16 06:33:30 by aartiges         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/includes.h"

uint16_t	compute_icmp_checksum(const void *buff, int length)
{
	const uint16_t	*ptr = buff;
	uint32_t		sum;

	sum = 0;
	while (length > 1)
	{
		sum += *ptr++;
		length -= 2;
	}
	if (length == 1)
		sum += *(uint8_t *) ptr;
	sum = (sum >> 16) + (sum & 0xffff);
	sum += (sum >> 16);
	return (~sum);
}

int	ft_atoi(const char *str)
{
	long	result;
	long	sign;

	result = 0;
	sign = 1;
	if (*str == '-')
	{
		sign = -1;
		str++;
	}
	else if (*str == '+')
		str++;
	while (*str >= '0' && *str <= '9')
	{
		if (result > INT_MAX)
			return (0);
		result = result * 10 + (*str - '0');
		str++;
	}
	return ((int)(result * sign));
}

double	get_diff_tv(struct timeval *tv_recv, struct timeval *tv_send)
{
	struct timeval	tv_diff;

	tv_diff.tv_sec = tv_recv->tv_sec - tv_send->tv_sec;
	tv_diff.tv_usec = tv_recv->tv_usec - tv_send->tv_usec;
	return (
		(double)tv_diff.tv_sec * 1000.
		+ (double)tv_diff.tv_usec / 1000.);
}

int	is_finish(t_traceroute *traceroute)
{
	if (traceroute->ttl >= 255 || traceroute->ttl > traceroute->max_ttl)
		return (1);
	return (0);
}
