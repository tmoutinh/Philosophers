/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoutinh <tmoutinh@student.42porto.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/16 00:46:23 by tmoutinh          #+#    #+#             */
/*   Updated: 2023/09/09 19:57:01 by tmoutinh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Philo.h"

void	finisher(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->nb_philo)
		pthread_join(data->philo[i].philo, NULL);
	pthread_join(data->watcher, NULL);
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
	data->dead = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
	if (!data->forks || !data->philo || !data->meal || !data->write)
		return (-2);
	pthread_mutex_init(data->meal, NULL);
	pthread_mutex_init(data->write, NULL);
	pthread_mutex_init(data->finish, NULL);
	pthread_mutex_init(data->dead, NULL);
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
	if (ft_strcmp(status, DIE) == 0)
	{
		pthread_mutex_lock(data->write);
		printf("%lld %d %s\n", get_time() - data->start_time, philo->right_fork + 1, status);
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
			printf("%lld %d %s\n", get_time() - data->start_time, philo->right_fork + 1, status);
			pthread_mutex_unlock(data->write);
		}
		else
			pthread_mutex_unlock(data->dead);
	}
}

void	meal(void *arg)
{
	t_data	*data;
	t_philo	*philo;

	philo = (t_philo *)arg;
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
	pthread_mutex_lock(data->finish);
	philo->eaten_nb += 1;
	philo->t_lasteat = get_time();
	pthread_mutex_unlock(data->finish);
}

void	execute(void *arg)
{
	t_data	*data;
	t_philo	*philo;

	philo = (t_philo*)arg;
	data = philo->data;

	print_action(philo, THINK);
	meal(arg);
	pthread_mutex_unlock(&data->forks[philo->left_fork]);
	pthread_mutex_unlock(&data->forks[philo->right_fork]);
	print_action(philo, SLEEP);
	usleep(data->t_slp * 1000);
}

void	*action(void *arg)
{
	t_data	*data;
	t_philo	*philo;

	philo = (t_philo*)arg;
	data = philo->data;
	pthread_mutex_lock(data->dead);
	// if  (data->nb_eats > 0)
	// {
	// 	while (philo->eaten_nb < data->nb_eats
	// 		&& data->rip_flag == 1)
	// 	{
	// 		pthread_mutex_unlock(data->dead);
	// 		execute(arg);
	// 		pthread_mutex_lock(data->dead);
	// 	}
	// }
	// else
	// {
		while (data->rip_flag == 1)
		{
			pthread_mutex_unlock(data->dead);
			execute(arg);
			pthread_mutex_lock(data->dead);
		}
	//}
	pthread_mutex_unlock(data->dead);
	return (NULL);
}

int	dead_man(t_data *data, int *i)
{
	if (*i == data->nb_philo)
		*i = 0;
	pthread_mutex_lock(data->finish);
	if (get_time() - data->philo[*i].t_lasteat >= data->t_die)
	{
		pthread_mutex_lock(data->dead);
		data->rip_flag = 0;
		print_action(&data->philo[*i], DIE);
		pthread_mutex_unlock(data->dead);
		pthread_mutex_unlock(data->finish);
		return (1);
	}
	*i += 1;
	pthread_mutex_unlock(data->finish);
	return (0);
}

int	full_break(t_data *data, int i,int *j)
{
	t_philo *philo;

	philo = &data->philo[i];
	if (data->nb_eats > 0 && philo->eaten_nb == data->nb_eats)
		*j += 1;
	// else
	// 	*j = 0;
	//printf("i is %d and j is %d\n", i, *j);
	if (*j == data->nb_philo - 1)
	{
		pthread_mutex_lock(data->dead);
		data->rip_flag = 0;
		return (0);
	}
	return (1);
}

void	*inspect(void	*arg)
{
	int	i;
	int	j;
	t_data	*data;
	t_philo	*philo;

	i = 0;
	j = 0;
	data = (t_data*)arg;
	philo = &data->philo[i];
	while (data->rip_flag == 1)
	{
		pthread_mutex_lock(data->finish);
		if (full_break(data, i, &j) == 0)
		{
			pthread_mutex_unlock(data->dead);
			pthread_mutex_unlock(data->finish);
			break ;
		}
		pthread_mutex_unlock(data->finish);
		if (dead_man(data, &i) == 1)
			break ;
	}
	usleep(10);
	return (NULL);
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
	pthread_create(&data->watcher, NULL, inspect, data); // Not sure if the use of data->philo->philo is correct!
	finisher(data);
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