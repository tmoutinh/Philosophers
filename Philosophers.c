/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoutinh <tmoutinh@student.42porto.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/16 00:46:23 by tmoutinh          #+#    #+#             */
/*   Updated: 2023/08/17 18:16:35 by tmoutinh         ###   ########.fr       */
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
	t_data	*data;

	data = (t_data*)arg;
	printf("%lld philosopher %d is thinking\n", get_time(), data->philo->right_fork);
	printf("ok\n");
	pthread_mutex_lock(&data->forks[data->philo->right_fork]);
	pthread_mutex_lock(&data->forks[data->philo->left_fork]);
	printf("%lld philosopher %d is eating\n", get_time(), data->philo->right_fork);
	usleep(data->t_eat);
	pthread_mutex_unlock(&data->forks[data->philo->right_fork]);
	pthread_mutex_unlock(&data->forks[data->philo->left_fork]);
	printf("%lld philosopher %d is sleeping\n", get_time(), data->philo->right_fork);
	usleep(data->t_slp);
	return (NULL);
}

int	init_data(char **argv, t_data *data)
{
	data->nb_philo = ft_atoi(argv[1]);
	data->t_die = ft_atoi(argv[2]);
	data->t_eat = ft_atoi(argv[3]);
	data->t_slp = ft_atoi(argv[4]);
	if (data->nb_philo < 1 || data->t_die < 1
	|| data->t_eat < 1 || data->t_slp < 1)
		return (-1);
	return (1);
}

void	philo_init(t_philo *philo, t_data *data)
{
	int	i;

	i = -1;
	data->forks = (pthread_mutex_t *)malloc(sizeof(*data->forks) * data->nb_philo);
	if (!data->forks)
		return ;
	while (++i < data->nb_philo)
	{
		if (i == data->nb_philo - 1)
		{
			philo[i].right_fork = i;
			philo[i].left_fork = 0;	
		}
		philo[i].right_fork = i;
		philo[i].left_fork = i + 1;
		philo[i].t_lasteat = get_time();
		pthread_mutex_init(&data->forks[i], NULL);
	}
}

void	*someone_dead(void *arg)
{
	t_data	*data;
	int		i;

	data = (t_data*)arg;
	i = -1;
	while (get_time() - data->philo[++i].t_lasteat < data->t_die)
	{
		if (i == data->nb_philo - 1)
			i = -1;
	}
	printf("%lld philosopher %d has died\n", get_time(), i);
	return ((void *) -1); // Should return NULL?
}

void	philosophers(t_data *data)
{
	int	i;
	t_philo philo[data->nb_philo];
	pthread_t	time_to_die;

	i = -1;
	data->philo = philo;
	philo_init(data->philo, data);
	pthread_create(&time_to_die, NULL, someone_dead, &data);
	pthread_detach(time_to_die);
	while (++i < data->nb_philo)
	{
		pthread_create(&(data->philo[i].philo), NULL, action, &data); // Not sure if the use of data->philo->philo is correct!
	}
	i = -1;
	while (++i < data->nb_philo)
	{
		pthread_join(data->philo[i].philo, NULL);
	}
}

int	main(int argc, char **argv)
{
	t_data	data;

	if (argc < 5 || argc > 6)
		return (5);
	if (init_data(argv, &data) == -1)
		return (-1);
	philosophers(&data);
	printf("%d\n", data.nb_philo);
	return (0);
}