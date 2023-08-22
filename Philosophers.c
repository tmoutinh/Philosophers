/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoutinh <tmoutinh@student.42porto.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/16 00:46:23 by tmoutinh          #+#    #+#             */
/*   Updated: 2023/08/22 01:24:39 by tmoutinh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Philo.h"

void	finisher(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->nb_philo)
		pthread_join(data->philo[i].philo, NULL);
	i = -1;
	while (++i < data->nb_philo)	
		pthread_mutex_destroy(&data->forks[i]);
	pthread_mutex_destroy(data->write);
	pthread_mutex_destroy(data->meal);
}

int	init_data(int argc, char **argv, t_data *data)
{
	data->nb_philo = ft_atoi(argv[1]);
	data->t_die = ft_atoi(argv[2]);
	data->t_eat = ft_atoi(argv[3]);
	data->t_slp = ft_atoi(argv[4]);
	data->start_time = get_time();
	if (argc == 6)
		data->nb_eats = ft_atoi(argv[5]);
	else
		data->nb_eats = -1;
	if (data->nb_philo < 1 || data->t_die < 1
	|| data->t_eat < 1 || data->t_slp < 1 ||
	 (argc == 6 && data->nb_eats < 1))
		return (-1);
	data->philo = (t_philo *)malloc(sizeof(t_philo) * data->nb_philo);
	data->forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * data->nb_philo);
	data->meal = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
	data->write = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
	data->finish = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
	if (!data->forks || !data->philo || !data->meal || !data->write)
		return (-2);
	pthread_mutex_init(data->meal, NULL);
	pthread_mutex_init(data->write, NULL);
	pthread_mutex_init(data->finish, NULL);
	return (1);
}
void	philo_init(t_data *data)
{
	int	i;

	i = -1;
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
void	print_action(t_philo *arg, char *status)
{
	t_data	*data;
	t_philo	*philo;

	philo = (t_philo*)arg;
	data = philo->data;
	
	pthread_mutex_lock(data->write);
	printf("%lld %d %s\n", get_time() - data->start_time, philo->right_fork + 1, status);
	pthread_mutex_unlock(data->write);
}

void	meal(t_philo *arg)
{
	t_data	*data;
	t_philo	*philo;

	philo = arg;
	data = philo->data;
	
	if (philo->right_fork % 2 == 0)
	{
		pthread_mutex_lock(&data->forks[philo->left_fork]);
		pthread_mutex_lock(&data->forks[philo->right_fork]);
	}
	else
	{
		pthread_mutex_lock(&data->forks[philo->right_fork]);
		pthread_mutex_lock(&data->forks[philo->left_fork]);
	}
	print_action(arg, TAKE);
	print_action(arg, TAKE);
	print_action(arg, EAT);
	usleep(data->t_eat * 1000);
	pthread_mutex_lock(data->meal);
	philo->eaten_nb += 1;
	philo->t_lasteat = get_time();
	pthread_mutex_unlock(data->meal);
}

void	*action(void *arg)
{
	t_data	*data;
	t_philo	*philo;

	philo = (t_philo*)arg;
	data = philo->data;
	while (1)
	{
		if (data->rip_flag == 0)
			return (NULL);
		print_action(arg, THINK);
		meal(arg);
		pthread_mutex_unlock(&data->forks[philo->left_fork]);
		pthread_mutex_unlock(&data->forks[philo->right_fork]);
		print_action(arg, SLEEP);
		usleep(data->t_slp * 1000);
	}
	return (NULL);
}

int	inspect (void	*arg)
{
	int	i;
	t_data	*data;

	data = arg;
	//pthread_mutex_lock(data->finish);
	while (data->rip_flag == 1)
	{
		i = -1;
		while (++i < data->nb_philo)
		{
			//pthread_mutex_lock(data->meal);
			if (get_time() - data->philo[i].t_lasteat >= data->t_die)
			{
				data->rip_flag = 0;
				print_action(&data->philo[i], DIE);
			//	pthread_mutex_unlock(data->finish);
			//	pthread_mutex_unlock(data->meal);	
				return (0);
			}
			//pthread_mutex_unlock(data->meal);	
		}
		usleep(10);
	}
	//pthread_mutex_unlock(data->finish);
	finisher(data);
	return (1);
}

void	philosophers(t_data *data)
{
	int	i;

	i = -1;
	philo_init(data);
	while (++i < data->nb_philo)
	{		
		pthread_create(&(data->philo[i].philo), NULL, action, &data->philo[i]); // Not sure if the use of data->philo->philo is correct!
	}
	inspect(data);

}

int	main(int argc, char **argv)
{
	t_data	data;

	if (argc < 5 || argc > 6)
	{
		write(STDERR_FILENO, "Invalid number of arguments\n", 28);
		return (5);
	}
	if (init_data(argc, argv, &data) < 1)
	{
		write(STDERR_FILENO, "Invalid argument introduction\n", 30);
		return (-1);
	}	
	philosophers(&data);
	return (0);
}