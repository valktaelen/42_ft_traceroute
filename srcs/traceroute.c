/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   traceroute.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aartiges <aartiges@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/15 04:47:01 by aartiges          #+#    #+#             */
/*   Updated: 2023/04/16 03:52:13 by aartiges         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/includes.h"

int	send_traceroute(t_traceroute *traceroute)
{
	t_icmp_packet	icmp_pkt;
	struct timeval	tv;
	ssize_t			num_bytes_sent;
	int				packet_len;

	ft_bzero(&icmp_pkt, sizeof(icmp_pkt));
	icmp_pkt.type = ICMP_ECHO_REQUEST;
	icmp_pkt.code = 0;
	icmp_pkt.id = ft_htons(getpid());
	icmp_pkt.seq_num = ft_htons(++(traceroute->seq_num));
	gettimeofday(&tv, NULL);
	gettimeofday(&(traceroute->timeout), NULL);
	icmp_pkt.timestamp = tv;
	packet_len = ICMP_HEADER_LEN + sizeof(tv);
	icmp_pkt.checksum = 0;
	icmp_pkt.checksum = compute_icmp_checksum(&icmp_pkt, packet_len);
	num_bytes_sent = sendto(traceroute->sockfd, &icmp_pkt, packet_len, 0,
			(struct sockaddr*)(&(traceroute->host_addr)), sizeof(traceroute->host_addr));
	if (num_bytes_sent < 0)
	{
		if (DEBUG_EXEC)
			dprintf(2, "sendto: %s\n", strerror(errno));
		return (-1);
	}
	return (0);
}

static int	proceed_receive(t_traceroute_info *infos, char *buffer)
{
	gettimeofday(&(infos->tv_recv), NULL);
	ft_memcpy(&(infos->ip), buffer, sizeof(struct iphdr));
	ft_memcpy(&(infos->icmp_hdr), (buffer + sizeof(struct iphdr)),
		sizeof(struct icmphdr));
	if (infos->icmp_hdr.type != ICMP_ECHO_REPLY)
		return (-2);
	ft_memcpy(&(infos->icmp_pkt), (buffer + sizeof(struct iphdr)),
		infos->pkt_len - sizeof(struct iphdr));
	if (infos->icmp_pkt.type != ICMP_ECHO_REPLY
		|| infos->icmp_pkt.id != ft_htons(getpid()))
		return (-3);
	infos->rtt = get_diff_tv(&(infos->tv_recv), &(infos->icmp_pkt.timestamp));
	return (0);
}

static int	receive_traceroute(t_traceroute *traceroute, t_traceroute_info *infos)
{
	char			buffer[BUFFER_SIZE];
	socklen_t		addrlen;

	++infos->icmp_pkt.seq_num;
	traceroute->cur_addr.sa_family = traceroute->host_addr.sin_family;
	addrlen = sizeof(traceroute->cur_addr);
	infos->pkt_len = recvfrom(traceroute->sockfd, buffer, BUFFER_SIZE, 0,
			&(traceroute->cur_addr), &addrlen);
	if (infos->pkt_len < 0)
	{
		if (DEBUG_EXEC)
			dprintf(2, "recvfrom: %s\n", strerror(errno));
		return (-1);
	}
	return (proceed_receive(infos, buffer));
}

static int	ft_init_traceroute(t_traceroute *traceroute)
{
	int	ret;

	ret = init_socket(traceroute);
	if (ret)
		return (ret);
	printf("traceroute %s (%s) using protocol ICMP\n", traceroute->domain, traceroute->ip_str);
	return (0);
}

int	ft_traceroute_probes(t_traceroute *traceroute, t_traceroute_info *infos)
{
	int	ret;
	int	i;

	i = 0;
	while (i < traceroute->nqueries)
	{
		if (send_traceroute(traceroute) < 0)
			continue ;
		ret = receive_traceroute(traceroute, infos);
		if (ret == -1)
		{
			printf("* * *");
			break ;
		}
		else if (ret)
			print_error(traceroute);
		else
		{
			print_success(traceroute, infos);
			traceroute->end = 1;
		}
		++i;
		++(traceroute->ttl);
		if (i != traceroute->nqueries && !is_finish(traceroute) && !traceroute->end)
			usleep(1000000 * traceroute->sendwait.tv_sec);
		--(traceroute->ttl);
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
