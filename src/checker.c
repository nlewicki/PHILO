/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlewicki <nlewicki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 12:34:55 by nlewicki          #+#    #+#             */
/*   Updated: 2024/08/23 10:07:11 by nlewicki         ###   ########.fr       */
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
	t_philo	*current;
	int		done_eating;

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
		return (pthread_mutex_unlock(&current->data->lock), 0);
	}
	return (1);
}

size_t	get_current_time(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		write(2, "gettimeofday() error\n", 22);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

int	ft_usleep(size_t milliseconds)
{
	size_t	start;

	start = get_current_time();
	while ((get_current_time() - start) < milliseconds)
		usleep(500);
	return (0);
}
