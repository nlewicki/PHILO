/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlewicki <nlewicki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 10:58:25 by nlewicki          #+#    #+#             */
/*   Updated: 2024/08/23 12:02:25 by nlewicki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct s_philo	t_philo;

typedef struct s_data
{
	t_philo			*philo_list;
	int				alive;
	int				nb_philo;
	unsigned long	time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				nb_time_each_philo_must_eat;
	int				philos_done_eating;
	pthread_mutex_t	*forks;
	pthread_mutex_t	eat;
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
	pthread_mutex_t	*l_fork;
	pthread_mutex_t	*r_fork;
	t_data			*data;

}					t_philo;

void	print_msg(t_philo *philo, char *str);
int		ft_atoi(const char *str);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
size_t	get_current_time(void);
int		ft_usleep(size_t milliseconds);

int		check_health(t_data *data);
int		check_meals(t_data *data);

void	philo_eat(t_philo *philo);
void	philo_sleep(t_philo *philo);
void	philo_think(t_philo *philo);
void	*routine(void *arg);

int		check_philo(t_philo *philo);
void	*watcher_routine(void *philo);

int		check_input(int argc, char *argv[]);
void	init_data(int argc, char *argv[], t_data *data);
int		init_philo(t_data *data, t_philo *philo_list, pthread_t *thread);
int		init_fork_mutex(t_data *data);
int		init_threads(t_philo *philo);

void	destory_mutex(t_data *data);
void	cleanup(t_data *data, t_philo *philo_list, pthread_t *threads);

#endif
