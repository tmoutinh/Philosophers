/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoutinh <tmoutinh@student.42porto.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/16 00:46:23 by tmoutinh          #+#    #+#             */
/*   Updated: 2023/08/18 12:31:34 by tmoutinh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Philo.h"

unsigned long long get_time(void) 
{
    struct timeval tv;
	
    gettimeofday(&tv, NULL);
    return (unsigned long long)(tv.tv_sec) * 1000 + (unsigned long long)(tv.tv_usec) / 1000;
}


unsigned long long	ft_atoi(const char *nptr)
{
	unsigned long long	i;
	int	neg;

	i = 0;
	neg = 1;
	while (*nptr == 32 || (*nptr >= 9 && *nptr <= 13))
		nptr++;
	if (*nptr == '-' || *nptr == '+')
	{
		if (*nptr == '-')
			neg *= (-1);
		nptr++;
	}
	if (!(*nptr >= '0' && *nptr <= '9'))
		return (-1);
	while (*nptr >= '0' && *nptr <= '9')
	{
		i = i * 10 + (*nptr - 48);
		nptr++;
	}
	return (i * neg);
}

/*The right forks tells me the id of the philo*/
void	*action(void *arg)
{
//	printf("i am working ok %d\n", ((t_philo*)arg)->right_fork);
//	sleep(2);
	t_data	*data;
	t_philo	*philo;

	philo = (t_philo*)arg;
	data = philo->data;
	// long long int i;
	// unsigned long long int j;	
	// i = get_time();
	// j = 200;
	while (1)
	{
		printf("%lld %d is thinking\n", get_time() - data->start_time, philo->right_fork);
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
		printf("%lld %d is eating\n", get_time() - data->start_time, philo->right_fork);
		philo->t_lasteat = get_time();
		usleep(data->t_eat);
		pthread_mutex_unlock(&data->forks[philo->right_fork]);
		pthread_mutex_unlock(&data->forks[philo->left_fork]);
		printf("%lld %d is sleeping\n", get_time() - data->start_time, philo->right_fork);
		usleep(data->t_slp);
	}
	return (NULL);
}

int	init_data(char **argv, t_data *data)
{
	data->nb_philo = ft_atoi(argv[1]);
	data->t_die = ft_atoi(argv[2]);
	data->t_eat = ft_atoi(argv[3]);
	data->t_slp = ft_atoi(argv[4]);
	data->start_time = get_time();
	if (data->nb_philo < 1 || data->t_die < 1
	|| data->t_eat < 1 || data->t_slp < 1)
		return (-1);
	data->philo = (t_philo *)malloc(sizeof(t_philo) * data->nb_philo);
	data->forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * data->nb_philo);
	if (!data->forks || !data->philo)
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
		pthread_mutex_init(&data->forks[i], NULL);
		data->philo[i].data = data;
	}
}

void	*someone_dead(void *arg)
{
	t_data	*data;
	int		i;

	data = (t_data*)arg;
	i = -1;
	while (1)
	{
		if (get_time() - data->philo[++i].t_lasteat > data->t_die)
		{
			printf("%lld %d died\n", get_time() - data->start_time, i);		
			exit(EXIT_FAILURE);
		}
		if (i == data->nb_philo - 1)
			i = -1;
	}
	return (NULL);
}

void	philosophers(t_data *data)
{
	int	i;
	pthread_t	time_to_die;

	i = -1;
	philo_init(data);
	pthread_create(&time_to_die, NULL, someone_dead, data);
	pthread_detach(time_to_die);
	while (++i < data->nb_philo)
	{		
		pthread_create(&(data->philo[i].philo), NULL, action, &data->philo[i]); // Not sure if the use of data->philo->philo is correct!
	}
	i = -1;
	while (++i < data->nb_philo)
	{
		pthread_join(data->philo[i].philo, NULL);
	}
	i = -1;
	pthread_exit(&time_to_die);
	while (++i < data->nb_philo)
		pthread_mutex_destroy(&data->forks[i]);
}

int	main(int argc, char **argv)
{
	t_data	data;

	if (argc < 5 || argc > 6)
		return (5);
	if (init_data(argv, &data) == -1)
		return (-1);
	philosophers(&data);
	return (0);
}