/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoutinh <tmoutinh@student.42porto.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/16 00:46:23 by tmoutinh          #+#    #+#             */
/*   Updated: 2023/09/22 17:07:42 by tmoutinh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Philo.h"

void	sleeper(t_data *data, long long sleep)
{
	long long	begin;

	begin = get_time();
	pthread_mutex_lock(data->dead);
	while (data->rip_flag == 1 && get_time() - begin < sleep)
	{
		pthread_mutex_unlock(data->dead);
		usleep(10);
		pthread_mutex_lock(data->dead);
	}
	pthread_mutex_unlock(data->dead);
}

void	print_action(t_philo *philo, char *status, char *color)
{
	t_data	*data;

	data = philo->data;
	if (ft_strcmp(status, DIE) == 0)
	{
		pthread_mutex_lock(data->write);
		printf("%s%lld %d %s"RESET"\n", color, get_time() - data->start_time,
			philo->right_fork + 1, status);
		pthread_mutex_unlock(data->write);
	}
	else
	{
		pthread_mutex_lock(data->dead);
		if ((data->nb_eats > 0 && philo->eaten_nb < data->nb_eats
				&& data->rip_flag == 1) || data->rip_flag == 1)
		{
			pthread_mutex_unlock(data->dead);
			pthread_mutex_lock(data->write);
			printf("%s%lld %d %s"RESET"\n", color, get_time() - data->start_time,
				philo->right_fork + 1, status);
			pthread_mutex_unlock(data->write);
		}
		else
			pthread_mutex_unlock(data->dead);
	}
}

void	philosophers(t_data *data)
{
	int	i;

	i = -1;
	if (mutex_initializer(data) < 0)
		return ;
	philo_init(data);
	while (++i < data->nb_philo)
	{
		if (pthread_create(&(data->philo[i].philo), NULL,
				action, &data->philo[i]))
			exit_error (data, -5);
	}
	if (pthread_create(&data->watcher, NULL, inspect, data))
		finisher(data);
	finisher(data);
}

int	single_philo(t_data *data)
{
	printf("%lld 1 has taken a fork\n",
		get_time() - data->start_time);
	usleep(data->t_die * 1000);
	printf("%lld 1 died\n", data->t_die);
	return (1);
}

int	main(int argc, char **argv)
{
	t_data	data;

	if (argument_validation(argc, argv, &data) < 0)
		return (-1);
	if (data.nb_philo == 1)
		return (single_philo(&data));
	philosophers(&data);
	return (0);
}
