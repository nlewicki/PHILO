/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlewicki <nlewicki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 10:58:15 by nlewicki          #+#    #+#             */
/*   Updated: 2024/07/26 12:49:22 by nlewicki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	philo_eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->fork);
	print_msg(philo, "has taken a fork");
	pthread_mutex_lock(&philo->next->fork);
	print_msg(philo, "has taken a fork");
	philo->last_meal = get_current_time();
	print_msg(philo, "is eating");
	ft_usleep(philo->data->time_to_eat);
	pthread_mutex_unlock(&philo->fork);
	pthread_mutex_unlock(&philo->next->fork);
	decrement_meals(philo);
}

void	philo_sleep(t_philo *philo)
{
	print_msg(philo, "is sleeping");
	ft_usleep(philo->data->time_to_sleep);
}

void	philo_think(t_philo *philo)
{
	print_msg(philo, "is thinking");
}

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0 || philo->id == philo->data->nb_philo)
	{
		print_msg(philo, "is thinking");
		ft_usleep(philo->data->time_to_eat / 2);
	}
	while (check_health(philo) && philo->data->nb_philo > 1
		&& check_meals(philo))
	{
		philo_eat(philo);
		philo_sleep(philo);
		philo_think(philo);
	}
	return (NULL);
}

int	main(int argc, char *argv[])
{
	t_data	data;
	t_philo	*philo_list;

	philo_list = NULL;
	if (check_input(argc, argv))
		return (0);
	init_data(argc, argv, &data);
	init_philo(&data, &philo_list);
	watcher_routine(philo_list);
	join_and_destroy(philo_list, data);
	return (0);
}
