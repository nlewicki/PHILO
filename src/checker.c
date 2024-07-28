/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlewicki <nlewicki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 12:34:55 by nlewicki          #+#    #+#             */
/*   Updated: 2024/07/28 18:52:36 by nlewicki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_msg(t_philo *philo, char *str)
{
	size_t	time;

	time = get_current_time() - philo->start_time;
	pthread_mutex_lock(&philo->data->print);
	if (check_health(philo) && check_meals(philo))
		printf("%zu %d %s\n", time, philo->id, str);
	pthread_mutex_unlock(&philo->data->print);
}

int	check_health(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->lock);
	if (philo->data->alive == 1)
		return (pthread_mutex_unlock(&philo->data->lock), 1);
	pthread_mutex_unlock(&philo->data->lock);
	return (0);
}

int	check_meals(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->lock);
	if (philo->data->nb_time_each_philo_must_eat != -1
		&& philo->data->total_meals <= 0)
	{
		pthread_mutex_lock(&philo->data->eating);
		philo->data->philos_done_eating++;
		pthread_mutex_unlock(&philo->data->eating);
		if (philo->data->nb_philo == philo->data->philos_done_eating)
			philo->data->alive = 0;
		return (pthread_mutex_unlock(&philo->data->lock), 1);
	}
	return (pthread_mutex_unlock(&philo->data->lock), 1);
}

void	decrement_meals(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->lock);
	if (philo->data->nb_time_each_philo_must_eat != -1
		&& philo->data->total_meals > 0)
		philo->data->total_meals--;
	pthread_mutex_unlock(&philo->data->lock);
}
