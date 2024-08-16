/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlewicki <nlewicki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 10:58:25 by nlewicki          #+#    #+#             */
/*   Updated: 2024/08/16 11:38:57 by nlewicki         ###   ########.fr       */
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
	unsigned long	time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				nb_time_each_philo_must_eat;
	int				philos_done_eating;
	pthread_mutex_t	lock;
	pthread_mutex_t	print;
}					t_data;

typedef struct s_philo
{
	pthread_t		thread;
	int				id;
	size_t			last_meal;
	size_t			start_time;
	int				meals_eaten;
	pthread_mutex_t	fork;
	pthread_mutex_t	thread_lock;
	t_data			*data;
	struct s_philo	*next;

}					t_philo;

int		ft_atoi(const char *str);
size_t	get_current_time(void);
int		ft_usleep(size_t milliseconds);
int		check_input(int argc, char *argv[]);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
int		check_health(t_philo *philo);
int		check_meals(t_philo *philo);
void	print_msg(t_philo *philo, char *str);
void	init_data(int argc, char *argv[], t_data *data);
t_philo	*create_philo(int id, t_data *data);
void	decrement_meals(t_philo *philo);
void	philo_eat(t_philo *philo);
void	philo_sleep(t_philo *philo);
void	philo_think(t_philo *philo);
void	*routine(void *arg);
int		create_and_link_philos(t_data *data, t_philo **philo_list,
			t_philo *current, t_philo *tmp);
void	init_philo(t_data *data, t_philo **philo_list);
int		check_philo(t_philo *philo);
int		watcher_routine(t_philo *philo);
void	join_and_destroy(t_philo *philo_list, t_data data);

#endif
