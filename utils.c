/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoutinh <tmoutinh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/18 14:24:32 by tmoutinh          #+#    #+#             */
/*   Updated: 2023/09/10 23:59:39 by tmoutinh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Philo.h"

long long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((long long)(tv.tv_sec)*1000 + (long long)(tv.tv_usec) / 1000);
}

int	ft_strcmp(char *s1, char *s2)
{
	while (*s1 != '\0' && (*s1 == *s2))
	{
		s1++;
		s2++;
	}
	return (*(char *)s1 - *(char *)s2);
}

long long	ft_atoi(const char *nptr)
{
	long long	i;
	int			neg;

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
	if (i > __INT_MAX__)
		return (-1);
	return (i * neg);
}

void	finisher(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->nb_philo)
	{
		pthread_join(data->philo[i].philo, NULL);
		pthread_mutex_destroy(&data->forks[i]);
	}
	free(data->philo);
	free(data->forks);
	pthread_join(data->watcher, NULL);
	pthread_mutex_destroy(data->write);
	free(data->write);
	pthread_mutex_destroy(data->dead);
	free(data->dead);
	pthread_mutex_destroy(data->finish);
	free(data->finish);
}