/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gcabecas <gcabecas@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 12:13:31 by gcabecas          #+#    #+#             */
/*   Updated: 2026/03/31 11:01:09 by gcabecas         ###   ########lyon.fr   */
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

typedef struct s_request
{
	int			coder_id;
	long long	priority;
	int			granted;
}	t_request;

typedef struct s_heap
{
	t_request	*data;
	int			size;
	int			capacity;
}	t_queue;

typedef struct s_dongle
{
	pthread_mutex_t	mutex;
	pthread_cond_t	cond;
	long long		last_release;
	t_queue			queue;
	int				held;
}	t_dongle;

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
	int				compile_count;
	long long		last_compile_start;
	pthread_mutex_t	mutex;
	struct s_sim	*sim;
}	t_coder;

typedef struct s_sim
{
	t_args			*args;
	t_coder			*coders;
	t_dongle		*dongles;
	pthread_t		*threads;
	pthread_t		burnout;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	stop_mutex;
	int				stop;
	long long		start_time;
}	t_sim;

int			parse_args(int argc, char **argv, t_args *args);
t_sim		*init_sim(t_args *args);
void		free_sim(t_sim *sim);
int			take_dongles(t_coder *coder);
void		release_dongles(t_coder *coder);
void		release_one(t_coder *coder, int idx);
int			create_threads(t_sim *sim);
void		join_threads(t_sim *sim);
long long	get_time_ms(t_sim *sim);
void		print_log(t_sim *sim, int id, char *msg);
void		set_stop(t_sim *sim);
int			is_stopped(t_sim *sim);
void		set_compile_start(t_coder *coder, long long time);
long long	get_compile_start(t_coder *coder);
void		inc_compile_count(t_coder *coder);
int			get_compile_count(t_coder *coder);
void		*burnout_routine(void *arg);
int			queue_init(t_queue *h, int cap);
void		queue_destroy(t_queue *h);
void		queue_push(t_queue *h, t_request req);
t_request	queue_pop(t_queue *h);
t_request	*queue_peek(t_queue *h);
void		queue_sift_up(t_queue *h, int idx);
void		queue_sift_down(t_queue *h, int idx);

#endif