/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlewicki <nlewicki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 10:58:25 by nlewicki          #+#    #+#             */
/*   Updated: 2024/07/23 13:54:22 by nlewicki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct s_data
{
	int				alive;
	int				nb_philo;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				nb_time_each_philo_must_eat;
	pthread_mutex_t	print;
}					t_data;

typedef struct s_philo
{
	pthread_t		thread;
	int				id;
	size_t			last_meal;
	pthread_mutex_t	fork;
	t_data			*data;
	struct s_philo	*next;

}					t_philo;



#endif
