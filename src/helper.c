/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlewicki <nlewicki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 12:29:33 by nlewicki          #+#    #+#             */
/*   Updated: 2024/08/21 13:47:49 by nlewicki         ###   ########.fr       */
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

void	destory_mutex(t_data *data)
{
	pthread_mutex_destroy(&data->print);
	pthread_mutex_destroy(&data->lock);
}

void	join_and_destroy(t_philo *philo_list, t_data data)
{
	t_philo	*current;
	t_philo	*tmp;
	int i;

	i = data.nb_philo;
	i *= data.nb_time_each_philo_must_eat;
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
		pthread_mutex_destroy(&current->l_fork);
		pthread_mutex_destroy(&current->r_fork);
		free(current);
		current = tmp;
		if (current == philo_list)
			break ;
	}
	// destory_mutex(&data);
}
