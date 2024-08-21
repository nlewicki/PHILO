/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlewicki <nlewicki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 12:34:55 by nlewicki          #+#    #+#             */
/*   Updated: 2024/08/21 11:54:20 by nlewicki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_msg(t_philo *philo, char *str)
{
	size_t	time;

	pthread_mutex_lock(&philo->data->print);
	time = get_current_time() - philo->start_time;
	if (check_health(philo))
	{
		printf("%zu %d %s\n", time, philo->id, str);
	}
	pthread_mutex_unlock(&philo->data->print);
}

// int	check_meals(t_philo *philo)
// {
// 	pthread_mutex_lock(&philo->data->lock);
// 	if (philo->data->nb_time_each_philo_must_eat != -1)
// 	{
// 		if (philo->meals_eaten >= philo->data->nb_time_each_philo_must_eat)
// 		{
// 			philo->data->philos_done_eating++;
// 			if (philo->data->nb_philo == philo->data->philos_done_eating)
// 			{
// 				philo->data->alive = 0;
// 				pthread_mutex_unlock(&philo->data->lock);
// 				return (0);
// 			}
// 		}
// 	}
// 	pthread_mutex_unlock(&philo->data->lock);
// 	return (1);
// }

int	check_meals(t_philo *philo)
{
	t_philo	*current;
	int done_eating;

	current = philo;
	done_eating = 0;
	while (current)
	{
		if (current->data->nb_time_each_philo_must_eat != -1)
		{
			pthread_mutex_lock(&current->data->eat);
			if (current->meals_eaten >= current->data->nb_time_each_philo_must_eat)
				done_eating++;
			pthread_mutex_unlock(&current->data->eat);
		current = current->next;
		if (current == philo)
			break ;
		}
	}
	if (current->data->nb_philo == done_eating)
	{
		pthread_mutex_lock(&current->data->lock);
		current->data->alive = 0;
		pthread_mutex_unlock(&current->data->lock);
		return (0);
	}
	return (1);
}
