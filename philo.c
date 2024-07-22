/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlewicki <nlewicki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 10:58:15 by nlewicki          #+#    #+#             */
/*   Updated: 2024/07/22 11:44:47 by nlewicki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	"philo.h"

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

int	check_input(int argc, char *argv[])
{
	if (argc != 5 && argc != 6)
		return (write(2, "Error: wrong number of arguments\n", 33), 1);
	if (atoi(argv[1]) <= 0 || atoi(argv[1]) > 200)
		return (write(2, "Error: wrong number of philosophers\n", 37), 1);
	if (atoi(argv[2]) <= 0)
		return (write(2, "Error: wrong time to die\n", 26), 1);
	if (atoi(argv[3]) <= 0)
		return (write(2, "Error: wrong time to eat\n", 26), 1);
	if (atoi(argv[4]) <= 0)
		return (write(2, "Error: wrong time to sleep\n", 28), 1);
	if (argc == 6 && atoi(argv[5]) <= 0)
		return (write(2, "Error: wrong number of time each philosopher must eat\n", 55), 1);
	return (0);
}

void	init_data(int argc, char *argv[], t_data *data)
{
	data->nb_philo = atoi(argv[1]);
	data->time_to_die = atoi(argv[2]);
	data->time_to_eat = atoi(argv[3]);
	data->time_to_sleep = atoi(argv[4]);
	if (argc == 6)
		data->nb_time_each_philo_must_eat = atoi(argv[5]);
	else
		data->nb_time_each_philo_must_eat = -1;
	pthread_mutex_init(&data->print, NULL);
}

t_philo *create_philo(int id, t_data *data)
{
	t_philo	*new_philo;

	new_philo = malloc(sizeof(t_philo));
	if (!new_philo)
		return (NULL);
	new_philo->id = id;
	new_philo->data = data;
	pthread_mutex_init(&new_philo->fork, NULL);
	new_philo->next = NULL;
	return (new_philo);
}

void	lock_print(t_philo *philo, char *str, size_t time)
{
	pthread_mutex_lock(&philo->data->print);
	printf("%zu %d %s\n", time, philo->id, str);
	pthread_mutex_unlock(&philo->data->print);
}

void	philo_eat(t_philo *philo, size_t start)
{
	pthread_mutex_lock(&philo->fork);
	lock_print(philo, "has taken a fork", get_current_time() - start);
	pthread_mutex_lock(&philo->next->fork);
	lock_print(philo, "has taken a fork", get_current_time() - start);
	lock_print(philo, "is eating", get_current_time() - start);
	ft_usleep(philo->data->time_to_die);
	lock_print(philo, "is sleeping", get_current_time() - start);
	pthread_mutex_unlock(&philo->fork);
	pthread_mutex_unlock(&philo->next->fork);
	ft_usleep(philo->data->time_to_sleep);
}

void	*routine(void *arg)
{
	t_philo *philo = (t_philo *)arg;
	size_t start = get_current_time();
	int i = 0;

	if (philo->id % 2 == 0 || philo->id == philo->data->nb_philo)
	{
		lock_print(philo, "is thinking", get_current_time() - start);
		ft_usleep(philo->data->time_to_sleep);
	}
	while (i < 4)
	{
		philo_eat(philo, start);
		ft_usleep(philo->data->time_to_eat);
		i++;
	}

	return (NULL);
}

void	init_philo(int argc, char *argv[], t_data *data, t_philo **philo_list)
{
	int i = 1;
	t_philo *current = NULL;
	t_philo *tmp = NULL;

	while (i <= data->nb_philo)
	{
		t_philo *new_philo = create_philo(i, data);
		if (!new_philo)
		{
			while (*philo_list)
			{
				tmp = (*philo_list)->next;
				free(*philo_list);
				*philo_list = tmp;
			}
			return ;
		}
		if (!*philo_list)
			*philo_list = new_philo;
		else
			current->next = new_philo;
		current = new_philo;
		i++;
	}
	current->next = *philo_list;
	current = *philo_list;
	while (current)
	{
		pthread_create(&current->thread, NULL, routine, current);
		current = current->next;
		if(current == *philo_list)
			break;
	}
}

int	main(int argc, char *argv[])
{
	t_data data;
	t_philo	*philo_list;
	int i = 0;
	if (check_input(argc, argv))
		return (0);
	init_data(argc, argv, &data);
	init_philo(argc, argv, &data, &philo_list);

	t_philo *current = philo_list;
	while (current)
	{
		pthread_join(current->thread, NULL);
		current = current->next;
		if(current == philo_list)
			break;
	}

	current = philo_list;
	while (current)
	{
		t_philo *tmp = current->next;
		free(current);
		current = tmp;
		if(current == philo_list)
			break;
	}

	pthread_mutex_destroy(&data.print);
	return (0);
}

