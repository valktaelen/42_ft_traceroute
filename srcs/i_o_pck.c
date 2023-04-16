/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   i_o_pck.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aartiges <aartiges@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/16 06:48:14 by aartiges          #+#    #+#             */
/*   Updated: 2023/04/16 06:50:35 by aartiges         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/includes.h"

int	send_traceroute(t_traceroute *tr)
{
	t_icmp_packet	icmp_pkt;
	struct timeval	tv;
	ssize_t			num_bytes_sent;
	int				packet_len;

	ft_bzero(&icmp_pkt, sizeof(icmp_pkt));
	icmp_pkt.type = ICMP_ECHO_REQUEST;
	icmp_pkt.code = 0;
	icmp_pkt.id = ft_htons(getpid());
	icmp_pkt.seq_num = ft_htons(++(tr->seq_num));
	gettimeofday(&tv, NULL);
	gettimeofday(&(tr->timeout), NULL);
	icmp_pkt.timestamp = tv;
	packet_len = ICMP_HEADER_LEN + sizeof(tv);
	icmp_pkt.checksum = 0;
	icmp_pkt.checksum = compute_icmp_checksum(&icmp_pkt, packet_len);
	num_bytes_sent = sendto(tr->sockfd, &icmp_pkt, packet_len, 0,
			(struct sockaddr*)(&(tr->host_addr)), sizeof(tr->host_addr));
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

int	receive_traceroute(t_traceroute *traceroute,
	t_traceroute_info *infos)
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
