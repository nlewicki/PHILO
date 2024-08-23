/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlewicki <nlewicki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 12:32:22 by nlewicki          #+#    #+#             */
/*   Updated: 2024/08/23 11:59:14 by nlewicki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_input(int argc, char *argv[])
{
	if (argc != 5 && argc != 6)
		return (write(2, "Error: wrong number of arguments\n", 33), 1);
	if (ft_atoi(argv[1]) <= 0 || atoi(argv[1]) > 200)
		return (write(2, "Error: wrong number of philos\n", 31), 1);
	if (ft_atoi(argv[2]) <= 0)
		return (write(2, "Error: wrong time to die\n", 26), 1);
	if (ft_atoi(argv[3]) <= 0)
		return (write(2, "Error: wrong time to eat\n", 26), 1);
	if (ft_atoi(argv[4]) <= 0)
		return (write(2, "Error: wrong time to sleep\n", 28), 1);
	if (argc == 6 && ft_atoi(argv[5]) <= 0)
		return (write(2, "Error: wrong number of philo meals\n", 36), 1);
	return (0);
}

void	init_data(int argc, char *argv[], t_data *data)
{
	data->nb_philo = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	data->alive = 1;
	if (argc == 6)
		data->nb_time_each_philo_must_eat = ft_atoi(argv[5]);
	else
		data->nb_time_each_philo_must_eat = -1;
	pthread_mutex_init(&data->print, NULL);
	pthread_mutex_init(&data->lock, NULL);
	pthread_mutex_init(&data->eat, NULL);
}

int	init_philo(t_data *data, t_philo *philo_list, pthread_t *thread)
{
	int	i;

	i = 0;
	while (i < data->nb_philo)
	{
		philo_list[i].id = i + 1;
		philo_list[i].data = data;
		philo_list[i].last_meal = get_current_time();
		philo_list[i].start_time = get_current_time();
		philo_list[i].meals_eaten = 0;
		philo_list[i].thread = thread[i];
		philo_list[i].l_fork = &data->forks[i];
		philo_list[i].r_fork = &data->forks[(i + 1) % data->nb_philo];
		i++;
	}
	return (0);
}

int	init_fork_mutex(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_philo)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
			return (1);
		i++;
	}
	return (0);
}

int	init_threads(t_philo *philo)
{
	int			i;
	pthread_t	watcher;

	i = 0;
	if (pthread_mutex_init(&philo->data->lock, NULL) != 0)
		return (1);
	if (pthread_mutex_init(&philo->data->print, NULL) != 0)
		return (1);
	if (pthread_mutex_init(&philo->data->eat, NULL) != 0)
		return (1);
	if (pthread_create(&watcher, NULL, watcher_routine, philo->data) != 0)
		return (1);
	while (i < philo->data->nb_philo)
	{
		if (pthread_create(&philo[i].thread, NULL, routine, &philo[i])
			!= 0)
			return (1);
		i++;
	}
	if (pthread_join(watcher, NULL) != 0)
		return (1);
	return (0);
}
