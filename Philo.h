/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoutinh <tmoutinh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/16 00:46:42 by tmoutinh          #+#    #+#             */
/*   Updated: 2023/09/11 00:05:40 by tmoutinh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>

#define TAKE "has taken a fork"
#define EAT "is eating"
#define THINK "is thinking"
#define SLEEP "is sleeping"
#define	DIE	"died"
#define	FULL	"All philo are full"

typedef struct s_philo
{
	pthread_t		philo;
	int				right_fork;
	int				left_fork;
	int				eaten_nb;
	long long int	t_lasteat;
	struct s_data	*data;
}	t_philo;

typedef struct s_data
{
	int			nb_philo;
	long long	t_die;
	long long	t_eat;
	long long	t_slp;
	int			nb_eats;
	long long	start_time;
	int			rip_flag;
	pthread_t			watcher;
	t_philo				*philo;
	pthread_mutex_t 	*forks;
	pthread_mutex_t		*write;
	pthread_mutex_t		*finish;
	pthread_mutex_t		*dead;
}	t_data;

/*Philosophers*/
void	sleeper(t_data *data, long long sleep);
void	print_action(t_philo *arg, char *status);
void	philosophers(t_data *data);
int	single_philo(t_data *data);
int	main(int argc, char **argv);
/*initializer.c*/
int	argument_validation(int argc, char **argv, t_data *data);
int	mutex_initializer(t_data *data);
int	init_data(int argc, char **argv, t_data *data);
void	philo_init(t_data *data);
/*watcher.c*/
int	dead_man(t_data *data, int *i);
int	full_break(t_data *data, int i, int *j);
void	*inspect(void	*arg);
/*routine.c*/
void	meal(void *arg);
void	execute(void *arg);
void	*action(void *arg);
/*utils.c*/
long long	get_time(void);
int	ft_strcmp(char *s1, char *s2);
long long	ft_atoi(const char *nptr);
void	finisher(t_data *data);

#endif