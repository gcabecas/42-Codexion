/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gcabecas <gcabecas@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 12:13:31 by gcabecas          #+#    #+#             */
/*   Updated: 2026/03/30 12:58:02 by gcabecas         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct s_args
{
	int	nb_coders;
	int	time_to_burnout;
	int	time_to_compile;
	int	time_to_debug;
	int	time_to_refactor;
	int	nb_compiles;
	int	dongle_cooldown;
	int	is_edf;
}	t_args;

typedef struct s_coder
{
	int				id;
	struct s_sim	*sim;
}	t_coder;

typedef struct s_sim
{
	t_args			*args;
	t_coder			*coders;
	pthread_t		*threads;
	pthread_mutex_t	print_mutex;
	int				stop;
	long long		start_time;
}	t_sim;

int		parse_args(int argc, char **argv, t_args *args);
t_sim	*init_sim(t_args *args);
void	free_sim(t_sim *sim);
int		create_threads(t_sim *sim);
void	join_threads(t_sim *sim);

#endif