/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initializer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoutinh <tmoutinh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/10 23:59:52 by tmoutinh          #+#    #+#             */
/*   Updated: 2023/09/11 00:01:36 by tmoutinh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Philo.h"

int	argument_validation(int argc, char **argv, t_data *data)
{
	if (argc < 5 || argc > 6)
	{
		write(STDERR_FILENO, "Invalid number of arguments\n", 28);
		return (5);
	}
	if (init_data(argc, argv, data) < 1)
	{
		write(STDERR_FILENO, "Invalid argument introduction\n", 30);
		return (-1);
	}
	return (0);
}

int	mutex_initializer(t_data *data)
{
	data->forks = (pthread_mutex_t *)malloc(
			sizeof(pthread_mutex_t) * data->nb_philo);
	if (!data->forks)
		return (-2);
	data->write = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
	if (!data->write)
		return (-2);
	data->finish = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
	if (!data->finish)
		return (-2);
	data->dead = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
	if (!data->dead)
		return (-2);
	pthread_mutex_init(data->write, NULL);
	pthread_mutex_init(data->finish, NULL);
	pthread_mutex_init(data->dead, NULL);
	return (0);
}

int	init_data(int argc, char **argv, t_data *data)
{
	data->nb_philo = ft_atoi(argv[1]);
	data->t_die = ft_atoi(argv[2]);
	data->t_eat = ft_atoi(argv[3]);
	data->t_slp = ft_atoi(argv[4]);
	data->start_time = get_time();
	data->nb_eats = -1;
	if (argc == 6)
		data->nb_eats = ft_atoi(argv[5]);
	if (data->nb_philo < 1 || data->t_die < 1
		|| data->t_eat < 1 || data->t_slp < 1
		|| (argc == 6 && data->nb_eats < 1))
		return (-1);
	return (1);
}

void	philo_init(t_data *data)
{
	int	i;

	i = -1;
	data->philo = (t_philo *)malloc(sizeof(t_philo) * data->nb_philo);
	if (!data->philo)
		return ;
	while (++i < data->nb_philo)
	{
		if (i == data->nb_philo - 1)
		{
			data->philo[i].right_fork = i;
			data->philo[i].left_fork = 0;
		}
		else
		{
			data->philo[i].right_fork = i;
			data->philo[i].left_fork = i + 1;
		}
		data->philo[i].eaten_nb = 0;
		pthread_mutex_init(&data->forks[i], NULL);
		data->philo[i].data = data;
		data->rip_flag = 1;
		data->philo[i].t_lasteat = get_time();
	}
}