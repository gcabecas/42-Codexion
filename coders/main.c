/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gcabecas <gcabecas@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 12:13:47 by gcabecas          #+#    #+#             */
/*   Updated: 2026/03/30 12:13:59 by gcabecas         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	main(int argc, char **argv)
{
	t_args	args;

	if (!parse_args(argc, argv, &args))
	{
		fprintf(stderr, "Error: invalid arguments.\n");
		return (1);
	}
	return (0);
}
