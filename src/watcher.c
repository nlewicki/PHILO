/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   watcher.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlewicki <nlewicki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 12:31:13 by nlewicki          #+#    #+#             */
/*   Updated: 2024/08/19 12:38:45 by nlewicki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_philo(t_philo *philo)
{
	int	result;

	result = 0;
	if (get_current_time() - philo->last_meal >= philo->data->time_to_die)
	{
		print_msg(philo, "\033[31mdied\033[0m");
		pthread_mutex_lock(&philo->data->lock);
		philo->data->alive = 0;
		pthread_mutex_unlock(&philo->data->lock);
		result = 1;
	}
	return (result);
}

int	watcher_routine(t_philo *philo)
{
	t_philo	*current;

	current = philo;
	while (current->data->alive == 1)
	{
		current = philo;
		while (current)
		{
			if (check_philo(current))
				return (1);
			current = current->next;
			if (current == philo)
				break ;
		}
	}
	return (0);
}
