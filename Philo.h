/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoutinh <tmoutinh@student.42porto.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/16 00:46:42 by tmoutinh          #+#    #+#             */
/*   Updated: 2023/09/05 18:40:58 by tmoutinh         ###   ########.fr       */
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
	pthread_mutex_t 	*meal;
	pthread_mutex_t		*write;
	pthread_mutex_t		*finish;
	pthread_mutex_t		*dead;
}	t_data;


long long get_time(void);
long long	ft_atoi(const char *nptr);
void	finisher(t_data *data);
void	*inspect(void *arg);

#endif