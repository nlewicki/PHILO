/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   watcher.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlewicki <nlewicki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 12:31:13 by nlewicki          #+#    #+#             */
/*   Updated: 2024/08/21 11:35:46 by nlewicki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_health(t_philo *philo)
{
	int	alive;

	pthread_mutex_lock(&philo->data->lock);
	alive = philo->data->alive;
	pthread_mutex_unlock(&philo->data->lock);
	return (alive);
}

int	check_philo(t_philo *philo)
{
	int	result;

	result = 1;
	if (get_current_time() - philo->last_meal >= philo->data->time_to_die)
	{
		print_msg(philo, "\033[31mdied\033[0m");
		pthread_mutex_lock(&philo->data->lock);
		philo->data->alive = 0;
		pthread_mutex_unlock(&philo->data->lock);
		result = 0;
	}
	return (result);
}

int	watcher_routine(t_philo *philo)
{
	t_philo	*current;

	current = philo;
	while (check_health(current) == 1)
	{
		current = philo;
		while (current)
		{
			if (!check_philo(current) || !check_meals(current))
				return (1);
			current = current->next;
			if (current == philo)
				break ;
		}
	}
	return (0);
}
