/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoutinh <tmoutinh@student.42porto.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/16 00:46:23 by tmoutinh          #+#    #+#             */
/*   Updated: 2023/08/19 04:23:46 by tmoutinh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Philo.h"

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
	if (!data->forks || !data->philo || !data->meal || !data->write)
		return (-2);
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
		data->philo[i].t_lasteat = get_time();
		data->philo[i].eaten_nb = 0;
		pthread_mutex_init(&data->forks[i], NULL);
		data->philo[i].data = data;
	}
}

/*The right forks tells me the id of the philo*/
void	*action(void *arg)
{
	// printf("i am working ok %d\n", ((t_philo*)arg)->right_fork);
	// sleep(2);
	t_data	*data;
	t_philo	*philo;

	philo = (t_philo*)arg;
	data = philo->data;
	while (1)
	{
		pthread_mutex_lock(data->write);
		printf("%lld %d is thinking\n", get_time() - data->start_time, philo->right_fork + 1);
		pthread_mutex_unlock(data->write);
		if (philo->right_fork % 2 == 0)
		{
			pthread_mutex_lock(&data->forks[philo->right_fork]);
			pthread_mutex_lock(data->write);
			printf("%lld %d has taken a fork\n", get_time() - data->start_time, philo->right_fork + 1);
			pthread_mutex_unlock(data->write);
			pthread_mutex_lock(&data->forks[philo->left_fork]);
			pthread_mutex_lock(data->write);
			printf("%lld %d has taken a fork\n", get_time() - data->start_time, philo->right_fork + 1);
		}
		else
		{
			pthread_mutex_lock(&data->forks[philo->left_fork]);
			pthread_mutex_lock(data->write);
			printf("%lld %d has taken a fork\n", get_time() - data->start_time, philo->right_fork + 1);
			pthread_mutex_unlock(data->write);
			pthread_mutex_lock(&data->forks[philo->right_fork]);
			pthread_mutex_lock(data->write);
			printf("%lld %d has taken a fork\n", get_time() - data->start_time, philo->right_fork + 1);
		}
		printf("%lld %d is eating\n", get_time() - data->start_time, philo->right_fork + 1);
		pthread_mutex_unlock(data->write);
		pthread_mutex_lock(data->meal);
		philo->t_lasteat = get_time();
		philo->eaten_nb += 1;
		pthread_mutex_unlock(data->meal);
		usleep(data->t_eat * 1000);
		pthread_mutex_unlock(&data->forks[philo->left_fork]);
		pthread_mutex_unlock(&data->forks[philo->right_fork]);
		printf("%lld %d is sleeping\n", get_time() - data->start_time, philo->right_fork + 1);
		usleep(data->t_slp * 1000);
	}
	return (NULL);
}


void	*someone_dead_or_full(void *arg)// MIGHT NOT WORK! BUT IDEAS
{
	t_philo	*philo;
	t_data	*data;
	int		i;
	int		j;

	philo = (t_philo*)arg;
	data = philo->data; 
	i = -1;
	while (1)
	{
		if (get_time() - philo->t_lasteat >= data->t_die)
		{
			pthread_mutex_lock(data->write);
			printf("%lld %d died\n", get_time() - data->start_time, i + 1);
			return (NULL);
		//exit(EXIT_FAILURE);// switch for destroys
		}
		if (philo->eaten_nb == data->nb_eats)
		{
			pthread_mutex_lock(data->write);
			printf("%lld philosophers are full\n", get_time() - data->start_time);
			return (NULL);
		}
	}
	return (NULL);
}

void	philosophers(t_data *data)
{
	int	i;
	pthread_t	*time_to_die;

	i = -1;
	philo_init(data);
	time_to_die = (pthread_t *)malloc(sizeof(pthread_t) * data->nb_philo);
	while (++i < data->nb_philo)
	{		
		pthread_create(&(data->philo[i].philo), NULL, action, &data->philo[i]); // Not sure if the use of data->philo->philo is correct!
		pthread_create(&time_to_die[i], NULL, someone_dead_or_full, &data->philo[i]);
		pthread_detach(&time_to_die[i]);
	}
	i = -1;
	while (++i < data->nb_philo)
	{
		pthread_join(data->philo[i].philo, NULL);
	}
	i = -1;
//	pthread_exit(&time_to_die);
	while (++i < data->nb_philo)
		pthread_mutex_destroy(&data->forks[i]);
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