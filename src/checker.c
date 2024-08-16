/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlewicki <nlewicki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 12:34:55 by nlewicki          #+#    #+#             */
/*   Updated: 2024/08/16 11:40:25 by nlewicki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_msg(t_philo *philo, char *str)
{
	size_t	time;

	pthread_mutex_lock(&philo->data->lock);
	time = get_current_time() - philo->start_time;
	if (check_health(philo) && check_meals(philo))
	{
		pthread_mutex_lock(&philo->data->print);
		printf("%zu %d %s\n", time, philo->id, str);
		pthread_mutex_unlock(&philo->data->print);
	}
	pthread_mutex_unlock(&philo->data->lock);
}

int	check_health(t_philo *philo)
{
	int	alive;

	pthread_mutex_lock(&philo->data->lock);
	alive = philo->data->alive;
	pthread_mutex_unlock(&philo->data->lock);
	return (alive);
}

int	check_meals(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->lock);
	if (philo->data->nb_time_each_philo_must_eat != -1)
	{
		if (philo->meals_eaten >= philo->data->nb_time_each_philo_must_eat)
		{
			philo->data->philos_done_eating++;
			if (philo->data->nb_philo == philo->data->philos_done_eating)
			{
				philo->data->alive = 0;
				pthread_mutex_unlock(&philo->data->lock);
				return (0);
			}
		}
	}
	pthread_mutex_unlock(&philo->data->lock);
	return (1);
}

// void	decrement_meals(t_philo *philo)
// {
// 	pthread_mutex_lock(&philo->data->lock);
// 	if (philo->data->nb_time_each_philo_must_eat != -1
// 		&& philo->data->total_meals > 0)
// 		philo->data->total_meals--;
// 	pthread_mutex_unlock(&philo->data->lock);
// }
