/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoutinh <tmoutinh@student.42porto.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/16 00:46:42 by tmoutinh          #+#    #+#             */
/*   Updated: 2023/08/18 01:53:13 by tmoutinh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct s_philo
{
	pthread_t		philo;
	int				right_fork;
	int				left_fork;
	unsigned long long int	t_lasteat;
	struct s_data	*data;
}	t_philo;

typedef struct s_data
{
	int	nb_philo;
	unsigned long long	t_die;
	unsigned long long	t_eat;
	unsigned long long	t_slp;
	unsigned long long	start_time;
	t_philo				*philo;
	pthread_mutex_t 	*forks;
}	t_data;


#endif