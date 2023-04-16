/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   traceroute.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aartiges <aartiges@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/15 04:47:01 by aartiges          #+#    #+#             */
/*   Updated: 2023/04/16 06:48:32 by aartiges         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/includes.h"

static int	ft_init_traceroute(t_traceroute *traceroute)
{
	int	ret;

	ret = init_socket(traceroute);
	if (ret)
		return (ret);
	printf("traceroute %s (%s) using protocol ICMP\n",
		traceroute->domain, traceroute->ip_str);
	return (0);
}

static int	ft_traceroute_probes(t_traceroute *tr_ro, t_traceroute_info *infos)
{
	int	ret;
	int	i;

	i = 0;
	while (i < tr_ro->nqueries)
	{
		if (send_traceroute(tr_ro) < 0)
			continue ;
		ret = receive_traceroute(tr_ro, infos);
		if (ret == -1)
			printf("* ");
		else if (ret)
			print_error(tr_ro);
		else
		{
			print_success(tr_ro, infos);
			tr_ro->end = 1;
		}
		++i;
		++(tr_ro->ttl);
		if (i != tr_ro->nqueries && !is_finish(tr_ro) && !tr_ro->end)
			usleep(1000000 * tr_ro->sendwait.tv_sec);
		--(tr_ro->ttl);
	}
	return (0);
}

int	ft_traceroute(t_traceroute *traceroute)
{
	t_traceroute_info			infos;

	if (ft_init_traceroute(traceroute))
		return (1);
	ft_bzero(&infos, sizeof(t_traceroute_info));
	while (!is_finish(traceroute) && !traceroute->end)
	{
		printf("%d\t", traceroute->ttl);
		setsockopt(traceroute->sockfd, IPPROTO_IP, IP_TTL,
			&(traceroute->ttl), sizeof(traceroute->ttl));
		ft_traceroute_probes(traceroute, &infos);
		printf("\n");
		++(traceroute->ttl);
	}
	close(traceroute->sockfd);
	return (0);
}
