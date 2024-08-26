/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlewicki <nlewicki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 12:34:55 by nlewicki          #+#    #+#             */
/*   Updated: 2024/08/26 11:46:59 by nlewicki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_health(t_data *data)
{
	int	alive;

	pthread_mutex_lock(&data->lock);
	alive = data->alive;
	pthread_mutex_unlock(&data->lock);
	return (alive);
}

int	check_meals(t_data *data)
{
	int		i;
	int		done_eating;

	i = 0;
	done_eating = 0;
	while (i < data->nb_philo)
	{
		if (data->nb_time_each_philo_must_eat != -1)
		{
			pthread_mutex_lock(&data->eat);
			if (data->philo_list[i].meals_eaten
				>= data->nb_time_each_philo_must_eat)
				done_eating++;
			pthread_mutex_unlock(&data->eat);
		}
		i++;
	}
	if (data->nb_philo == done_eating)
	{
		pthread_mutex_lock(&data->lock);
		data->alive = 0;
		pthread_mutex_unlock(&data->lock);
		return (0);
	}
	return (1);
}
