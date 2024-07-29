/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlewicki <nlewicki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 12:32:22 by nlewicki          #+#    #+#             */
/*   Updated: 2024/07/29 12:10:16 by nlewicki         ###   ########.fr       */
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
	{
		data->nb_time_each_philo_must_eat = ft_atoi(argv[5]);
		data->total_meals = data->nb_time_each_philo_must_eat * data->nb_philo;
		data->philos_done_eating = 0;
	}
	else
	{
		data->nb_time_each_philo_must_eat = -1;
		data->total_meals = -1;
	}
	pthread_mutex_init(&data->print, NULL);
	pthread_mutex_init(&data->lock, NULL);
	pthread_mutex_init(&data->eating, NULL);
	pthread_mutex_init(&data->alive_lock, NULL);
	pthread_mutex_init(&data->check, NULL);
	pthread_mutex_init(&data->time, NULL);
}

int	create_and_link_philos(t_data *data, t_philo **philo_list,
	t_philo *current, t_philo *tmp)
{
	int		i;
	t_philo	*new_philo;

	i = 1;
	while (i <= data->nb_philo)
	{
		new_philo = create_philo(i, data);
		if (!new_philo)
		{
			while (*philo_list)
			{
				tmp = (*philo_list)->next;
				free(*philo_list);
				*philo_list = tmp;
			}
			return (-1);
		}
		if (!*philo_list)
			*philo_list = new_philo;
		else
			current->next = new_philo;
		current = new_philo;
		i++;
	}
	return (current->next = *philo_list, 0);
}

void	init_philo(t_data *data, t_philo **philo_list)
{
	t_philo	*current;
	t_philo	*tmp;

	current = NULL;
	tmp = NULL;
	if (create_and_link_philos(data, philo_list, current, tmp) == -1)
		return ;
	current = *philo_list;
	while (current)
	{
		pthread_mutex_lock(&current->thread_lock);
		pthread_create(&current->thread, NULL, routine, current);
		pthread_mutex_unlock(&current->thread_lock);
		current = current->next;
		if (current == *philo_list)
			break ;
	}
}

t_philo	*create_philo(int id, t_data *data)
{
	t_philo	*new_philo;

	new_philo = malloc(sizeof(t_philo));
	if (!new_philo)
		return (NULL);
	new_philo->id = id;
	new_philo->data = data;
	new_philo->last_meal = get_current_time();
	new_philo->start_time = get_current_time();
	pthread_mutex_init(&new_philo->fork, NULL);
	pthread_mutex_init(&new_philo->thread_lock, NULL);
	new_philo->next = NULL;
	return (new_philo);
}
