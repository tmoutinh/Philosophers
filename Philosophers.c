/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoutinh <tmoutinh@student.42porto.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/16 00:46:23 by tmoutinh          #+#    #+#             */
/*   Updated: 2023/08/16 23:58:38 by tmoutinh         ###   ########.fr       */
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


void	*action(void *arg)
{
	t_data	*data;

	data = (t_data*)arg;
	
	
}

int	init_data(char **argv, t_data *data)
{
	data->nb_philo = ft_atoi(argv[1]);
	data->t_die = ft_atoi(argv[2]);
	data->t_eat = ft_atoi(argv[3]);
	data->t_slp = ft_atoi(argv[4]);
	if (data->nb_philo < 0 || data->t_die < 0
	|| data->t_eat < 0 || data->t_slp < 0)
		return (-1);
	return (1);
}

void	philo_init(t_philo *philo, int nb_philo)
{
	int	i;

	i = -1;
	while (++i < nb_philo)
	{
		if (i == nb_philo - 1)
		{
			philo[i].right_fork = i;
			philo[i].left_fork = 0;	
		}
		philo[i].right_fork = i;
		philo[i].left_fork = i + 1;
		philo[i].t_lasteat = get_time();
	}
}

void	philosophers(t_data *data)
{
	int	i;
	t_philo philo[data->nb_philo];

	i = -1;
	philo_init(&philo, data->nb_philo);
	while (++i < data->nb_philo)
	{
		pthread_create(&(philo[i].philo), NULL, action, &philo);
	}
	i = -1;
	while (++i < data->nb_philo)
	{
		pthread_join(philo[i].philo, NULL);
	}

}

int	main(int argc, char **argv)
{
	t_data	data;

	if (argc < 5)
		return (5);
	if (init_data(argv, &data) == -1)
		return (-1);
	philosophers(&data);
	printf("%d\n", data.nb_philo);
	return (0);
}