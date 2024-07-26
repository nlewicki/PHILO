/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlewicki <nlewicki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 10:58:15 by nlewicki          #+#    #+#             */
/*   Updated: 2024/07/26 11:06:16 by nlewicki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_atoi(const char *str)
{
	int	i;
	int	sign;
	int	result;

	sign = 1;
	i = 0;
	result = 0;
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
	{
		i++;
	}
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign *= -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - 48);
		i++;
	}
	return (result * sign);
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

int	check_input(int argc, char *argv[])
{
	if (argc != 5 && argc != 6)
		return (write(2, "Error: wrong number of arguments\n", 33), 1);
	if (ft_atoi(argv[1]) <= 0 || atoi(argv[1]) > 200)
		return (write(2, "Error: wrong number of philos\n", 37), 1);
	if (ft_atoi(argv[2]) <= 0)
		return (write(2, "Error: wrong time to die\n", 26), 1);
	if (ft_atoi(argv[3]) <= 0)
		return (write(2, "Error: wrong time to eat\n", 26), 1);
	if (ft_atoi(argv[4]) <= 0)
		return (write(2, "Error: wrong time to sleep\n", 28), 1);
	if (argc == 6 && ft_atoi(argv[5]) <= 0)
		return (write(2, "Error: wrong number of philo meals\n", 55), 1);
	return (0);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	if (n == 0)
		return (0);
	while (i < (n - 1) && s1[i] != '\0' && s2[i] != '\0')
	{
		if (s1[i] != s2[i])
		{
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		}
		i++;
	}
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
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
		return (pthread_mutex_unlock(&philo->data->lock), 0);
	return (pthread_mutex_unlock(&philo->data->lock), 1);
}

void	print_msg(t_philo *philo, char *str)
{
	size_t	time;

	time = get_current_time() - philo->start_time;
	pthread_mutex_lock(&philo->data->print);
	if (check_health(philo) && check_meals(philo))
		printf("%zu %d %s\n", time, philo->id, str);
	pthread_mutex_unlock(&philo->data->print);
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
	}
	else
	{
		data->nb_time_each_philo_must_eat = -1;
		data->total_meals = -1;
	}
	pthread_mutex_init(&data->print, NULL);
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
	new_philo->next = NULL;
	return (new_philo);
}

void	decrement_meals(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->lock);
	if (philo->data->nb_time_each_philo_must_eat != -1
		&& philo->data->total_meals > 0)
		philo->data->total_meals--;
	pthread_mutex_unlock(&philo->data->lock);
}

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

void	init_philo(int argc, char *argv[], t_data *data, t_philo **philo_list)
{
	t_philo	*current;
	t_philo	*tmp;

	if (create_and_link_philos(data, philo_list, current, tmp) == -1)
		return ;
	current = *philo_list;
	while (current)
	{
		pthread_create(&current->thread, NULL, routine, current);
		current = current->next;
		if (current == *philo_list)
			break ;
	}
}

int	check_philo(t_philo *philo)
{
	if (get_current_time() - philo->last_meal >= philo->data->time_to_die)
	{
		print_msg(philo, "\033[31mdied\033[0m");
		philo->data->alive = 0;
		return (1);
	}
	return (0);
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

void	join_and_destroy(t_philo *philo_list, t_data data)
{
	t_philo	*current;
	t_philo	*tmp;

	current = philo_list;
	while (current)
	{
		pthread_join(current->thread, NULL);
		current = current->next;
		if (current == philo_list)
			break ;
	}
	current = philo_list;
	while (current)
	{
		tmp = current->next;
		pthread_mutex_destroy(&current->fork);
		free(current);
		current = tmp;
		if (current == philo_list)
			break ;
	}
	pthread_mutex_destroy(&data.print);
}

int	main(int argc, char *argv[])
{
	t_data	data;
	t_philo	*philo_list;

	philo_list = NULL;
	if (check_input(argc, argv))
		return (0);
	init_data(argc, argv, &data);
	init_philo(argc, argv, &data, &philo_list);
	watcher_routine(philo_list);
	join_and_destroy(philo_list, data);
	return (0);
}
