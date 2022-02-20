/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tevelyne <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/19 16:37:49 by tevelyne          #+#    #+#             */
/*   Updated: 2021/07/19 18:06:48 by tevelyne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# define RED     "\x1b[31m"
# define GREEN   "\x1b[32m"
# define YELLOW  "\x1b[33m"
# define RESET   "\x1b[0m"
# define MAX_INT 2147483647

# include	<stdio.h>
# include	<pthread.h>
# include	<errno.h>
# include	<string.h>
# include	<stdlib.h>
# include	<unistd.h>
# include	<sys/time.h>
# include    <inttypes.h>
# include	<semaphore.h>

typedef struct s_philosopher {
	int					phil_nbr;
	int					mls_nbr;
	int					ready;
	int					even;
	uint64_t			last_meal;
	uint64_t			death_t;
	pthread_mutex_t		*left;
	pthread_mutex_t		*right;
	struct s_variables	*main_s;
	pthread_t			flow;
}	t_philosopher;

typedef struct s_variables {
	int				nbr_of_phils;
	u_int64_t		time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				num_times_must_eat;
	uint64_t		start;
	pthread_mutex_t	msg;
	pthread_mutex_t	*forks;
	t_philosopher	*data_phil;
	pthread_mutex_t	m_died;
	pthread_t		*thread;
}	t_variables;

void		init_philosophers(t_variables *var, int i);
void		initialize(t_variables *var, int i);
void		*death_cycle(void *arg);
void		*end_holywar(void *arg);
void		*observation(t_philosopher	*data_phil);
int			ft_atoi(const char *str);
uint64_t	current_time(void);
void		free_close(t_variables *var, int i, int b);
void		message(t_philosopher *data_phil, char *msg);
void		meal(t_philosopher *data_phil);
void		*philo_state(void *arg);
int			ft_str_isdigit(char *str);
int			main(int argc, char **argv);

#endif
