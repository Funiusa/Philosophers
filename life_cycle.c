/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   life_cycle.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tevelyne <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/19 17:00:23 by tevelyne          #+#    #+#             */
/*   Updated: 2021/07/19 18:07:10 by tevelyne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	init_philosophers(t_variables *var, int i)
{
	while (i < var->nbr_of_phils)
	{
		var->data_phil[i].phil_nbr = i + 1;
		if ((var->data_phil[i].phil_nbr % 2) == 1)
			var->data_phil[i].even = 1;
		else
			var->data_phil[i].even = 0;
		var->data_phil[i].ready = 1;
		var->data_phil[i].mls_nbr = 0;
		var->data_phil[i].left = &var->forks[i];
		var->data_phil[i].right = &var->forks[(i + 1) % var->nbr_of_phils];
		var->data_phil[i].main_s = var;
		i++;
	}
	var->start = current_time();
}

void	initialize(t_variables *var, int i)
{
	var->thread = (pthread_t *)malloc(var->nbr_of_phils * sizeof(pthread_t));
	var->forks = (pthread_mutex_t *)malloc(var->nbr_of_phils
			* sizeof(pthread_mutex_t));
	var->data_phil = (t_philosopher *)malloc(sizeof(t_philosopher)
			* var->nbr_of_phils);
	pthread_mutex_init(var->forks, NULL);
	pthread_mutex_init(&var->msg, NULL);
	pthread_mutex_init(&var->m_died, NULL);
	pthread_mutex_lock(&var->m_died);
	while (i < var->nbr_of_phils)
	{
		pthread_mutex_init(&var->forks[i], NULL);
		i++;
	}
}

void	meal(t_philosopher *data_phil)
{
	message(data_phil, "[%s%lums%s] [Philosopher №_%d] [reflects]\n");
	pthread_mutex_lock(data_phil->left);
	message(data_phil, "[%s%lums%s] [Philosopher №_%d] [took the left fork]\n");
	pthread_mutex_lock(data_phil->right);
	message(data_phil,
		"[%s%lums%s] [Philosopher №_%d] [took the right fork]\n");
	pthread_mutex_lock(&data_phil->main_s->msg);
	printf("[%s%lums%s] [Philosopher №_%d] [%seating%s]\n",
		YELLOW, current_time() - data_phil->main_s->start, RESET,
		data_phil->phil_nbr, GREEN, RESET);
	pthread_mutex_unlock(&data_phil->main_s->msg);
	usleep(data_phil->main_s->time_to_eat * 1000);
	pthread_mutex_unlock(data_phil->left);
	pthread_mutex_unlock(data_phil->right);
	data_phil->mls_nbr++;
	data_phil->death_t = current_time() + data_phil->main_s->time_to_die;
	message(data_phil, "[%s%lums%s] [Philosopher №_%d] [is sleeping]\n");
	usleep(data_phil->main_s->time_to_sleep * 1000);
}

void	*philo_state(void *arg)
{
	t_philosopher	*data_phil;

	data_phil = (t_philosopher *)arg;
	if (observation(data_phil) != 0)
		return ((void *)1);
	while (data_phil->mls_nbr != data_phil->main_s->num_times_must_eat
		&& data_phil->ready)
		meal(data_phil);
	usleep(100);
	pthread_mutex_lock(&data_phil->main_s->msg);
	pthread_mutex_unlock(&data_phil->main_s->m_died);
	return (NULL);
}
