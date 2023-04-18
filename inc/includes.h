/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   includes.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aartiges <aartiges@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/27 05:55:55 by aartiges          #+#    #+#             */
/*   Updated: 2023/04/16 06:49:52 by aartiges         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef INCLUDES_H
# define INCLUDES_H

# include <errno.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <netdb.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <netinet/ip.h>
# include <netinet/ip_icmp.h>
# include <time.h>
# include <sys/time.h>
# include <netdb.h>
# include <signal.h>
# include <limits.h>

# define ICMP_ECHO_REQUEST 8
# define ICMP_ECHO_REPLY 0
# define ICMP_HEADER_LEN 8
# define BUFFER_SIZE 1024
# define DEBUG_PARSING 0
# define DEBUG_EXEC 0

typedef struct s_icmp_packet {
	uint8_t			type;
	uint8_t			code;
	uint16_t		checksum;
	uint16_t		id;
	uint16_t		seq_num;
	struct timeval	timestamp;
}	t_icmp_packet;

typedef struct s_traceroute
{
	t_icmp_packet		icmp_template;
	struct sockaddr_in	host_addr;
	struct sockaddr		cur_addr;
	struct timeval		timeout;
	double				rtt;
	int					sockfd;
	int					end;
	int					seq_num;
	int					count;
	int					ttl;
	int					max_ttl;
	int					nqueries;
	struct timeval		sendwait;
	uint8_t				trad_name;
	char				ip_str[INET_ADDRSTRLEN];
	char				*domain;
}	t_traceroute;

typedef struct s_traceroute_info
{
	struct iphdr	ip;
	ssize_t			pkt_len;
	double			rtt;
	t_icmp_packet	icmp_pkt;
	struct icmphdr	icmp_hdr;
	struct timeval	tv_recv;
}	t_traceroute_info;

t_traceroute	*get_traceroute(void);

int				parsing(const int argc, const char **argv,
					t_traceroute *traceroute);

int				send_traceroute(t_traceroute *tr);
int				receive_traceroute(t_traceroute *traceroute,
					t_traceroute_info *infos);

int				ft_traceroute(t_traceroute *traceroute);

int				init_socket(t_traceroute *traceroute);

uint16_t		compute_icmp_checksum(const void *buff, int length);
double			get_diff_tv(struct timeval *tv_recv, struct timeval *tv_send);
int				is_finish(t_traceroute *traceroute);
int				ft_atoi(const char *str);

void			display_help(int fd);
void			print_error(t_traceroute *traceroute);
void			print_success(t_traceroute *traceroute,
					t_traceroute_info *info);

void			*ft_memcpy(void *dst, const void *src, size_t n);
void			ft_bzero(void *s, size_t n);
size_t			ft_strlen(const char *str);
char			*ft_strcpy(char *dst, const char *src);

uint16_t		ft_htons(uint16_t n);
uint16_t		ft_ntohs(uint16_t n);
const char		*ft_inet_ntop(int af, const void *src, char *dst,
					socklen_t size);

#endif
