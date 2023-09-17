/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoutinh <tmoutinh@student.42porto.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/10 23:57:40 by tmoutinh          #+#    #+#             */
/*   Updated: 2023/09/17 18:29:27 by tmoutinh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Philo.h"

void	meal(t_philo *philo)
{
	t_data	*data;

	data = philo->data;
	if (philo->right_fork % 2 == 0)
	{
		pthread_mutex_lock(&data->forks[philo->left_fork]);
		pthread_mutex_lock(&data->forks[philo->right_fork]);
	}
	else
	{
		if (data->nb_philo % 2 != 0 && data->t_eat - data->t_slp >= 0)
			sleeper(data, data->t_eat - data->t_slp + 10);
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

void	*action(void *arg)
{
	t_data	*data;
	t_philo	*philo;

	philo = (t_philo *)arg;
	data = philo->data;
	print_action(philo, THINK);
	if (philo->right_fork % 2 != 0)
		usleep(data->t_eat * 1000);
	while (1)
	{
		pthread_mutex_lock(data->dead);
		if (data->rip_flag == 0)
		{
			pthread_mutex_unlock(data->dead);
			return (NULL);
		}
		pthread_mutex_unlock(data->dead);
		meal(philo);
		print_action(philo, SLEEP);
		sleeper(data, data->t_slp);
		print_action(philo, THINK);
	}
	return (NULL);
}
