/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aartiges <aartiges@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/15 04:07:39 by aartiges          #+#    #+#             */
/*   Updated: 2023/04/16 06:44:06 by aartiges         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/includes.h"

void	display_help(int fd)
{
	dprintf(fd, "\nUsage\nft_traceroute [options] <destination>\n\
\nOptions:\n\
\t<destination>\tdns name or ip address\n\
\t-h\t\thelp\n\
\t-f\t\tfirst_ttl <integer> default 1\n\
\t-m\t\tmax_ttl <integer> default 30\n\
\t-q\t\tnqueries <integer> Sets the number of probe packets per hop. The default is 3\n\
\t-z\t\tsendwait <integer> Minimal  time  interval between probes (default 0).\n\
\t-n\t\tDo not try to map IP addresses to host names when displaying them\n\
\t-4\t\tipv4 only\n\
\n");
}

void	print_success(t_traceroute *traceroute, t_traceroute_info *info)
{
	if (!traceroute->trad_name)
	{
		printf("%s %.2lf ms ", traceroute->ip_str, info->rtt);
		return ;
	}
	printf("%s", traceroute->ip_str);
	printf(" %.2lf ms ", info->rtt);
}

void	print_error(t_traceroute *traceroute)
{
	struct sockaddr_in	*addr;
	char				ip_str[INET_ADDRSTRLEN];
	struct timeval		tv;
	double				rtt;

	gettimeofday(&tv, NULL);
	rtt = get_diff_tv(&tv, &(traceroute->timeout));
	addr = (struct sockaddr_in *)(&traceroute->cur_addr);
	ft_bzero(ip_str, INET_ADDRSTRLEN);
	ft_inet_ntop(AF_INET, &(addr->sin_addr), ip_str, INET_ADDRSTRLEN);
	if (!traceroute->trad_name)
	{
		printf("%s %.2lf ms ", ip_str, rtt);
		return ;
	}
	printf("%s", ip_str);
	printf(" %.2lf ms ", rtt);
}
