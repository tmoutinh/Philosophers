/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoutinh <tmoutinh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/10 23:57:40 by tmoutinh          #+#    #+#             */
/*   Updated: 2023/09/11 15:27:57 by tmoutinh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Philo.h"

void	meal(t_philo *philo)
{
	t_data	*data;

	data = philo->data;
	if (philo->right_fork % 2 == 0)
	{
		usleep(500);
		pthread_mutex_lock(&data->forks[philo->left_fork]);
		pthread_mutex_lock(&data->forks[philo->right_fork]);
	}
	else
	{
		pthread_mutex_lock(&data->forks[philo->right_fork]);
		pthread_mutex_lock(&data->forks[philo->left_fork]);
	}
	print_action(philo, TAKE);
	print_action(philo, TAKE);
	print_action(philo, EAT);
	pthread_mutex_lock(data->finish);
	philo->eaten_nb += 1;
	philo->t_lasteat = get_time();
	pthread_mutex_unlock(data->finish);
	sleeper(data, data->t_eat);
	pthread_mutex_unlock(&data->forks[philo->left_fork]);
	pthread_mutex_unlock(&data->forks[philo->right_fork]);
}

void	execute(void *arg)
{
	t_data	*data;
	t_philo	*philo;

	philo = (t_philo *)arg;
	data = philo->data;
	print_action(philo, THINK);
	meal(arg);
	print_action(philo, SLEEP);
	sleeper(data, data->t_slp);
}

void	*action(void *arg)
{
	t_data	*data;
	t_philo	*philo;

	philo = (t_philo *)arg;
	data = philo->data;
	pthread_mutex_lock(data->dead);
	while (data->rip_flag == 1)
	{
		pthread_mutex_unlock(data->dead);
		execute(arg);
		pthread_mutex_lock(data->dead);
	}
	pthread_mutex_unlock(data->dead);
	return (NULL);
}
