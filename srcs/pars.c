/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aartiges <aartiges@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/15 01:46:16 by aartiges          #+#    #+#             */
/*   Updated: 2023/04/16 05:29:06 by aartiges         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/includes.h"

static int	parsing_time(struct timeval *tv, const char **argv, int *i,
	const char *flag)
{
	if (argv[*i + 1] == NULL)
		return (1);
	tv->tv_sec = ft_atoi(argv[*i + 1]);
	if (DEBUG_PARSING)
		printf("%s: %ld %ld\n", flag, tv->tv_sec, tv->tv_usec);
	if (!tv->tv_sec && !tv->tv_usec)
		return (1);
	++(*i);
	return (0);
}

static int	parsing_int(int *val, const char **argv, int *i,
	const char *flag)
{
	if (argv[*i + 1] == NULL)
		return (1);
	*val = ft_atoi(argv[*i + 1]);
	if (DEBUG_PARSING)
		printf("%s: %d\n", flag, *val);
	if (!*val)
		return (1);
	++(*i);
	return (0);
}

static int	parse_param(const char **argv, t_traceroute *traceroute, int *i)
{
	if (argv[*i][1] == 'f')
	{
		if (parsing_int(&(traceroute->ttl), argv, i, "first_ttl"))
			return (1);
	}
	else if (argv[*i][1] == 'm')
	{
		if (parsing_int(&(traceroute->max_ttl), argv, i, "max_ttl"))
			return (1);
	}
	else if (argv[*i][1] == 'q')
	{
		if (parsing_int(&(traceroute->nqueries), argv, i, "nqueries"))
			return (1);
	}
	else if (argv[*i][1] == 'z')
	{
		if (parsing_time(&(traceroute->sendwait), argv, i, "sendwait"))
			return (1);
	}
	return (0);
}

static int	iter_flags(const char **argv, t_traceroute *traceroute, int *i)
{
	if (!argv[*i][1] || argv[*i][2])
	{
		dprintf(2, "Error: Unknown flag %s\n", argv[*i]);
		return (1);
	}
	if ((argv[*i][1] == 'f') || (argv[*i][1] == 'm')
	|| (argv[*i][1] == 'q') || (argv[*i][1] == 'z'))
	{
		if (parse_param(argv, traceroute, i))
			return (1);
	}
	else if (argv[*i][1] == 'n')
		traceroute->trad_name = 0;
	else if (argv[*i][1] != '4')
		return (1);
	return (0);
}

int	parsing(const int argc, const char **argv, t_traceroute *traceroute)
{
	int	i;

	i = 1;
	while (i < argc)
	{
		if (argv[i][0] != '-')
		{
			if (traceroute->domain == NULL)
				traceroute->domain = (char *) argv[i];
			else
				return (1);
		}
		else
		{
			if (iter_flags(argv, traceroute, &i))
				return (1);
		}
		++i;
	}
	if (traceroute->domain == NULL)
		return ((dprintf(2, "Usage error: destination require\n") & 0) + 2);
	if (traceroute->nqueries < 1 || traceroute->ttl < 1 || traceroute->max_ttl < 1)
		return (1);
	return (0);
}
