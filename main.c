/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tevelyne <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/19 16:44:07 by tevelyne          #+#    #+#             */
/*   Updated: 2021/07/19 16:55:01 by tevelyne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	init_vars(t_variables *var)
{
	var->nbr_of_phils = 0;
	var->num_times_must_eat = -1;
	var->time_to_sleep = 0;
	var->time_to_eat = 0;
	var->time_to_die = 0;
}

int	check_arguments(char **argv)
{
	int	i;

	i = 1;
	while (argv[i])
	{
		if (ft_str_isdigit(argv[i]))
			return (1);
		i++;
	}
	return (0);
}

int	arguments(t_variables	*var, char **argv)
{
	init_vars(var);
	var->nbr_of_phils = ft_atoi(argv[1]);
	var->time_to_die = ft_atoi(argv[2]);
	var->time_to_eat = ft_atoi(argv[3]);
	var->time_to_sleep = ft_atoi(argv[4]);
	if (argv[5])
	{
		var->num_times_must_eat = ft_atoi(argv[5]);
		if (var->num_times_must_eat > MAX_INT || var->num_times_must_eat < 1)
		{
			printf("Error: Incorrect argument.\n");
			return (1);
		}
	}
	if (var->time_to_die < 1 || var->time_to_eat < 1 || var->time_to_sleep < 1
		|| var->time_to_die > MAX_INT || var->time_to_eat > MAX_INT
		|| var->time_to_sleep > MAX_INT || var->nbr_of_phils > 200
		|| var->nbr_of_phils < 1)
	{
		printf("Error: Incorrect argument.\n");
		return (1);
	}
	return (0);
}

int	treads_create(t_variables *var, int i)
{
	pthread_t	thread;

	init_philosophers(var, 0);
	if (var->num_times_must_eat > 0)
		if (pthread_create(&thread, NULL, &end_holywar, var) != 0)
			return (1);
	while (i < var->nbr_of_phils)
	{
		var->data_phil[i].death_t = current_time() + var->time_to_die;
		if (pthread_create(&var->data_phil[i].flow, NULL, &philo_state,
				(void *)&var->data_phil[i]) != 0)
			return (1);
		pthread_detach(var->data_phil[i].flow);
		i++;
	}
	pthread_mutex_lock(&var->m_died);
	return (0);
}

int	main(int argc, char **argv)
{
	t_variables	var;

	if (argc < 5 || argc > 6 || check_arguments(argv))
	{
		printf("Error: incorrect arguments!\n");
		return (1);
	}
	if (arguments(&var, argv) == 1)
		return (1);
	initialize(&var, 0);
	treads_create(&var, 0);
	free_close(&var, 0, 0);
	return (0);
}
