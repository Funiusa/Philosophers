/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utility.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tevelyne <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/19 16:37:49 by tevelyne          #+#    #+#             */
/*   Updated: 2021/07/19 18:06:48 by tevelyne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	message(t_philosopher *data_phil, char *msg)
{
	pthread_mutex_lock(&data_phil->main_s->msg);
	printf(msg, YELLOW, current_time() - data_phil->main_s->start, RESET,
		data_phil->phil_nbr);
	pthread_mutex_unlock(&data_phil->main_s->msg);
}

uint64_t	current_time(void)
{
	struct timeval	t;
	uint64_t		time;

	gettimeofday(&t, NULL);
	time = (t.tv_sec * 1000 + t.tv_usec / 1000);
	return (time);
}

void	free_close(t_variables *var, int i, int b)
{
	while (i < var->nbr_of_phils)
	{
		pthread_detach(var->data_phil[i].flow);
		i++;
	}
	while (b < var->nbr_of_phils)
	{
		pthread_mutex_destroy(&var->forks[b]);
		b++;
	}
	pthread_mutex_destroy(var->forks);
	pthread_mutex_destroy(&var->msg);
	free(var->thread);
	free(var->forks);
	free(var->data_phil);
}

int	ft_str_isdigit(char *str)
{
	while (*str)
	{
		if (*str > 47 && *str < 58)
			str++;
		else
			return (1);
	}
	return (0);
}

int	ft_atoi(const char *str)
{
	int	i;
	int	n;
	int	num;

	i = 0;
	n = 1;
	num = 0;
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			n = -1;
		i++;
	}
	while ('0' <= str[i] && str[i] <= '9' && str[i] != '\0')
	{
		num = num * 10 + (str[i] - '0');
		i++;
	}
	return (num * n);
}
