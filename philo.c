/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlewicki <nlewicki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 10:58:15 by nlewicki          #+#    #+#             */
/*   Updated: 2024/07/17 10:29:32 by nlewicki         ###   ########.fr       */
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
}

void	init_philo()

int	main(int argc, char *argv[])
{
	t_data data;
	t_philo	philo;
	size_t	start;
	start = get_current_time();
	int i = 0;
	if (check_input(argc, argv))
		return (0);
	init_data(argc, argv, &data);
	init_philo(argc, argv, &data, &philo);

	printf("%zu\n", get_current_time() - start);
	return (0);
}


