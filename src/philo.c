/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlewicki <nlewicki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 10:58:15 by nlewicki          #+#    #+#             */
/*   Updated: 2024/08/19 11:43:55 by nlewicki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	philo_eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->fork);
	print_msg(philo, "has taken a fork");
	pthread_mutex_lock(&philo->next->fork);
	print_msg(philo, "has taken a fork");
	pthread_mutex_lock(&philo->data->lock);
	philo->last_meal = get_current_time();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->data->lock);
	print_msg(philo, "is eating");
	ft_usleep(philo->data->time_to_eat);
	pthread_mutex_unlock(&philo->fork);
	pthread_mutex_unlock(&philo->next->fork);
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
	while (1)
	{
		printf("Debug: Philo %d before checks\n", philo->id);
        if (!check_health(philo))
        {
            printf("Debug: Philo %d failed health check\n", philo->id);
            break;
        }
        if (!check_meals(philo))
        {
            printf("Debug: Philo %d failed meals check\n", philo->id);
            break;
        }
		printf("debug\n");
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
	printf("Debug: Philosophers initialized\n");
	ft_usleep(100);
	watcher_routine(philo_list);
	join_and_destroy(philo_list, data);
	return (0);
}
