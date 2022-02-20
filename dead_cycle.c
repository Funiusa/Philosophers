/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dead_cycle.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tevelyne <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/19 17:01:17 by tevelyne          #+#    #+#             */
/*   Updated: 2021/07/19 18:10:10 by tevelyne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	*death_cycle(void *arg)
{
	t_philosopher	*phil;

	phil = (t_philosopher *)arg;
	while (1)
	{
		if (current_time() >= phil->death_t)
		{
			pthread_mutex_lock(&phil->main_s->msg);
			printf("[%s%lums%s] [Philosopher №_%d] [%sdied...%s]\n",
				   YELLOW, current_time() - phil->main_s->start,
				   RESET, phil->phil_nbr, RED, RESET);
			pthread_mutex_unlock(&phil->main_s->m_died);
			return (NULL);
		}
		usleep(1000);
	}
}

void	*end_holywar(void *arg)
{
	t_variables	*var;
	int			i;

	var = (t_variables *)arg;
	while (1)
	{
		i = 0;
		while (i < var->nbr_of_phils)
		{
			if (var->data_phil[i].mls_nbr < var->num_times_must_eat)
				break ;
			else if (var->data_phil[i].mls_nbr == var->num_times_must_eat)
				var->data_phil[i].ready = 0;
			i++;
		}
		if (i == var->nbr_of_phils)
			break ;
		usleep(1000);
	}
	pthread_mutex_lock(&var->msg);
	pthread_mutex_unlock(&var->m_died);
	return (NULL);
}

void	*observation(t_philosopher	*data_phil)
{
	pthread_t		observer;

	if (data_phil->even)
	{
		data_phil->even = 0;
		usleep(data_phil->main_s->time_to_eat);
	}
	data_phil->last_meal = current_time();
	data_phil->death_t = data_phil->last_meal + data_phil->main_s->time_to_die;
	if (pthread_create(&observer, NULL, &death_cycle, data_phil) != 0)
		return ((void *)1);
	pthread_detach(observer);
	return (0);
}
