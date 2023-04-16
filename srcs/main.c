/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aartiges <aartiges@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/27 06:03:57 by aartiges          #+#    #+#             */
/*   Updated: 2023/04/16 06:42:02 by aartiges         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/includes.h"

int	main(int argc, const char *argv[])
{
	t_traceroute	*traceroute;
	int				ret;

	traceroute = get_traceroute();
	ft_bzero(traceroute, sizeof(t_traceroute));
	traceroute->timeout.tv_sec = 1;
	traceroute->ttl = 1;
	traceroute->max_ttl = 30;
	traceroute->nqueries = 3;
	traceroute->trad_name = 1;
	ret = parsing(argc, argv, traceroute);
	if (ret == 1)
		display_help(2);
	if (ret)
		return (1);
	ft_traceroute(traceroute);
	return (0);
}
