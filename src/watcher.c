/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   watcher.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlewicki <nlewicki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 12:31:13 by nlewicki          #+#    #+#             */
/*   Updated: 2024/08/23 10:07:05 by nlewicki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

void	*watcher_routine(void *philo)
{
	t_philo	*current;

	current = (t_philo *)philo;
	while (check_health(current) == 1)
	{
		current = (t_philo *)philo;
		while (current)
		{
			if (!check_philo(current) || !check_meals(current))
				break ;
			current = current->next;
			if (current == (t_philo *)philo)
				break ;
		}
	}
	return (NULL);
}
