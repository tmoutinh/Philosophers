/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   watcher.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoutinh <tmoutinh@student.42porto.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/10 23:58:33 by tmoutinh          #+#    #+#             */
/*   Updated: 2023/09/17 17:32:45 by tmoutinh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Philo.h"

int	dead_man(t_data *data, int *i)
{
	if (*i == data->nb_philo)
		*i = 0;
	pthread_mutex_lock(data->finish);
	if (get_time() - data->philo[*i].t_lasteat >= data->t_die)
	{
		pthread_mutex_unlock(data->finish);
		pthread_mutex_lock(data->dead);
		data->rip_flag = 0;
		print_action(&data->philo[*i], DIE);
		pthread_mutex_unlock(data->dead);
		return (1);
	}
	*i += 1;
	pthread_mutex_unlock(data->finish);
	return (0);
}

int	full_break(t_data *data, int i, int *j)
{
	t_philo	*philo;

	philo = &data->philo[i];
	if (data->nb_eats > 0 && philo->eaten_nb == data->nb_eats)
		*j += 1;
	else if (data->nb_eats > 0 && philo->eaten_nb < data->nb_eats)
		*j = 0;
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
	int		i;
	int		j;
	t_data	*data;

	i = 0;
	j = 0;
	data = (t_data *)arg;
	while (1)
	{
		pthread_mutex_lock(data->finish);
		if (full_break(data, i, &j) == 0)
		{
			pthread_mutex_unlock(data->dead);
			pthread_mutex_unlock(data->finish);
			return (NULL);
		}
		pthread_mutex_unlock(data->finish);
		if (dead_man(data, &i) == 1)
			return (NULL);
	}
	usleep(10);
	return (NULL);
}
