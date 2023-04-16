/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aartiges <aartiges@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/15 03:01:14 by aartiges          #+#    #+#             */
/*   Updated: 2023/04/16 05:33:42 by aartiges         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/includes.h"

static int	iter_addr(t_traceroute *traceroute, struct addrinfo *result,
	struct addrinfo *addr, struct sockaddr_in *sa_in)
{
	addr = result;
	while (addr != NULL)
	{
		sa_in = (struct sockaddr_in *) addr->ai_addr;
		traceroute->host_addr = *sa_in;
		inet_ntop(AF_INET, &(traceroute->host_addr.sin_addr), traceroute->ip_str, \
		INET_ADDRSTRLEN);
		traceroute->sockfd = socket(addr->ai_family, SOCK_RAW, IPPROTO_ICMP);
		if (traceroute->sockfd < 0)
		{
			if (DEBUG_EXEC)
				dprintf(2, "socket: %s\n", strerror(errno));
			addr = addr->ai_next;
			continue ;
		}
		break ;
	}
	freeaddrinfo(result);
	if (!addr)
	{
		dprintf(2, "Could not create socket\n");
		return (-1);
	}
	return (0);
}

static int	set_socket(t_traceroute *traceroute)
{
	if (setsockopt(traceroute->sockfd, SOL_SOCKET, SO_RCVTIMEO,
			&(traceroute->timeout), sizeof(traceroute->timeout)) < 0)
	{
		dprintf(2, "setsockopt: %s\n", strerror(errno));
		close(traceroute->sockfd);
		return (1);
	}
	return (0);
}

int	init_socket(t_traceroute *traceroute)
{
	struct addrinfo		hints;
	struct addrinfo		*result;
	int					ret;

	ft_bzero(&hints, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_RAW;
	hints.ai_protocol = IPPROTO_ICMP;
	ret = getaddrinfo(traceroute->domain, NULL, &hints, &result);
	if (ret)
	{
		dprintf(2, "getaddrinfo: %s\n", gai_strerror(ret));
		return (-1);
	}
	ret = iter_addr(traceroute, result, NULL, NULL);
	if (ret)
		return (ret);
	return (set_socket(traceroute));
}
