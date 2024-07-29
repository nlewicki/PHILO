/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlewicki <nlewicki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 12:34:55 by nlewicki          #+#    #+#             */
/*   Updated: 2024/07/29 11:29:43 by nlewicki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_msg(t_philo *philo, char *str)
{
	size_t	time;

	pthread_mutex_lock(&philo->data->time);
	time = get_current_time() - philo->start_time;
	pthread_mutex_unlock(&philo->data->time);
	if (check_health(philo) && check_meals(philo))
	{
		pthread_mutex_lock(&philo->data->print);
		printf("%zu %d %s\n", time, philo->id, str);
		pthread_mutex_unlock(&philo->data->print);
	}
}

int	check_health(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->alive_lock);
	if (philo->data->alive == 1)
	{
		pthread_mutex_unlock(&philo->data->alive_lock);
		return (1);
	}
	pthread_mutex_unlock(&philo->data->alive_lock);
	return (0);
}

int	check_meals(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->check);
	if (philo->data->nb_time_each_philo_must_eat != -1
		&& philo->data->total_meals <= 0)
	{
		pthread_mutex_lock(&philo->data->eating);
		philo->data->philos_done_eating++;
		pthread_mutex_unlock(&philo->data->eating);
		if (philo->data->nb_philo == philo->data->philos_done_eating)
			philo->data->alive = 0;
		return (pthread_mutex_unlock(&philo->data->check), 1);
	}
	return (pthread_mutex_unlock(&philo->data->check), 1);
}

void	decrement_meals(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->lock);
	if (philo->data->nb_time_each_philo_must_eat != -1
		&& philo->data->total_meals > 0)
		philo->data->total_meals--;
	pthread_mutex_unlock(&philo->data->lock);
}
