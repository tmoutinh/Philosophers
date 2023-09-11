/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoutinh <tmoutinh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/16 00:46:42 by tmoutinh          #+#    #+#             */
/*   Updated: 2023/09/11 15:27:17 by tmoutinh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>

# define EAT "is eating"
# define DIE	"died"
# define FULL	"All philo are full"
# define SLEEP "is sleeping"
# define TAKE "has taken a fork"
# define THINK "is thinking"

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
	int					nb_philo;
	long long			t_die;
	long long			t_eat;
	long long			t_slp;
	int					nb_eats;
	long long			start_time;
	int					rip_flag;
	pthread_t			watcher;
	t_philo				*philo;
	pthread_mutex_t		*forks;
	pthread_mutex_t		*write;
	pthread_mutex_t		*finish;
	pthread_mutex_t		*dead;
}	t_data;

/*initializer.c*/
int			argument_validation(int argc, char **argv, t_data *data);
int			init_data(int argc, char **argv, t_data *data);
int			mutex_initializer(t_data *data);
void		philo_init(t_data *data);
/*Philosophers*/
int			main(int argc, char **argv);
void		philosophers(t_data *data);
void		print_action(t_philo *arg, char *status);
int			single_philo(t_data *data);
void		sleeper(t_data *data, long long sleep);
/*routine.c*/
void		*action(void *arg);
void		execute(void *arg);
void		meal(t_philo *philo);
/*utils.c*/
void		finisher(t_data *data);
long long	ft_atoi(const char *nptr);
int			ft_strcmp(char *s1, char *s2);
long long	get_time(void);
/*watcher.c*/
int			dead_man(t_data *data, int *i);
int			full_break(t_data *data, int i, int *j);
void		*inspect(void	*arg);

#endif