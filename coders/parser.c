/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gcabecas <gcabecas@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 12:14:15 by gcabecas          #+#    #+#             */
/*   Updated: 2026/03/30 21:36:51 by gcabecas         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	is_valid_uint(char *str)
{
	int		i;
	size_t	len;

	i = 0;
	if (!str || !str[0])
		return (0);
	len = strlen(str);
	if (len > 10)
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	if (len == 10 && strcmp(str, "2147483647") > 0)
		return (0);
	return (1);
}

static int	parse_one(char *str, int *out, int min)
{
	if (!is_valid_uint(str))
		return (0);
	*out = atoi(str);
	if (*out < min)
		return (0);
	return (1);
}

static int	parse_nums(char **argv, t_args *args)
{
	if (!parse_one(argv[1], &args->nb_coders, 1))
		return (0);
	if (!parse_one(argv[2], &args->time_to_burnout, 1))
		return (0);
	if (!parse_one(argv[3], &args->time_to_compile, 0))
		return (0);
	if (!parse_one(argv[4], &args->time_to_debug, 0))
		return (0);
	if (!parse_one(argv[5], &args->time_to_refactor, 0))
		return (0);
	if (!parse_one(argv[6], &args->nb_compiles, 1))
		return (0);
	if (!parse_one(argv[7], &args->dongle_cooldown, 0))
		return (0);
	return (1);
}

static int	parse_scheduler(char *str, t_args *args)
{
	if (strcmp(str, "fifo") == 0)
		args->is_edf = 0;
	else if (strcmp(str, "edf") == 0)
		args->is_edf = 1;
	else
		return (0);
	return (1);
}

int	parse_args(int argc, char **argv, t_args *args)
{
	if (argc != 9)
		return (0);
	if (!parse_nums(argv, args))
		return (0);
	if (!parse_scheduler(argv[8], args))
		return (0);
	return (1);
}
