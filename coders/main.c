/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gcabecas <gcabecas@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 12:13:47 by gcabecas          #+#    #+#             */
/*   Updated: 2026/03/30 12:58:05 by gcabecas         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	main(int argc, char **argv)
{
	t_args	args;
	t_sim	*sim;

	if (!parse_args(argc, argv, &args))
	{
		fprintf(stderr, "Error: invalid arguments.\n");
		return (1);
	}
	sim = init_sim(&args);
	if (!sim)
		return (1);
	if (!create_threads(sim))
	{
		free_sim(sim);
		return (1);
	}
	join_threads(sim);
	free_sim(sim);
	return (0);
}
