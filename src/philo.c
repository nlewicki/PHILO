/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlewicki <nlewicki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 10:58:15 by nlewicki          #+#    #+#             */
/*   Updated: 2024/08/21 12:07:12 by nlewicki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	philo_eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->l_fork);
	print_msg(philo, "has taken a fork");
	pthread_mutex_lock(&philo->r_fork);
	print_msg(philo, "has taken a fork");
	pthread_mutex_lock(&philo->data->eat);
	philo->last_meal = get_current_time();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->data->eat);
	print_msg(philo, "is eating");
	ft_usleep(philo->data->time_to_eat);
	pthread_mutex_unlock(&philo->l_fork);
	pthread_mutex_unlock(&philo->r_fork);
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
	while (check_health(philo) == 1)
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
	data.forks = malloc(sizeof(pthread_mutex_t) * ft_atoi(argv[1]));
	init_data(argc, argv, &data);
	init_philo(&data, &philo_list);
	ft_usleep(100);
	watcher_routine(philo_list);
	join_and_destroy(philo_list, data);
	return (0);
}
