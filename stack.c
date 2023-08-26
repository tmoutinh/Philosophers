// int	inspect(void	*arg)
// {
// 	t_data	*data;
// 	t_philo	*philo;

// 	philo = (t_philo*)arg;
// 	data = philo->data;
// 	pthread_mutex_lock(data->finish);
// 	pthread_mutex_lock(data->meal);
// 	if (get_time() - philo->t_lasteat >= data->t_die)
// 	{
// 		data->rip_flag = 0;
// 		print_action(philo, DIE);
// 		pthread_mutex_unlock(data->finish);
// 		pthread_mutex_unlock(data->meal);
// 		return (0);
// 	}
// 	pthread_mutex_unlock(data->finish);
// 	pthread_mutex_unlock(data->meal);
// 	return (1);
// }













// /*The right forks tells me the id of the philo*/
// void	*action(void *arg)
// {
// 	// printf("i am working ok %d\n", ((t_philo*)arg)->right_fork);
// 	// sleep(2);
// 	t_data	*data;
// 	t_philo	*philo;

// 	philo = (t_philo*)arg;
// 	data = philo->data;
// 	while (1)
// 	{
// 		// if (someone_dead_or_full(data) == 0)
// 		// 	return (0);
// 		pthread_mutex_lock(data->write);
// 		printf("%lld %d is thinking\n", get_time() - data->start_time, philo->right_fork + 1);
// 		pthread_mutex_unlock(data->write);
// 		if (philo->right_fork % 2 == 0)
// 		{
// 			pthread_mutex_lock(&data->forks[philo->left_fork]);
// 			pthread_mutex_lock(data->write);
// 			printf("%lld %d has taken a fork\n", get_time() - data->start_time, philo->right_fork + 1);
// 			pthread_mutex_unlock(data->write);
// 			pthread_mutex_lock(&data->forks[philo->right_fork]);
// 			pthread_mutex_lock(data->write);
// 			printf("%lld %d has taken a fork\n", get_time() - data->start_time, philo->right_fork + 1);
// 		}
// 		else
// 		{
// 			pthread_mutex_lock(&data->forks[philo->right_fork]);
// 			pthread_mutex_lock(data->write);
// 			printf("%lld %d has taken a fork\n", get_time() - data->start_time, philo->right_fork + 1);
// 			pthread_mutex_unlock(data->write);
// 			pthread_mutex_lock(&data->forks[philo->left_fork]);
// 			pthread_mutex_lock(data->write);
// 			printf("%lld %d has taken a fork\n", get_time() - data->start_time, philo->right_fork + 1);
// 		}
// 		printf("%lld %d is eating\n", get_time() - data->start_time, philo->right_fork + 1);
// 		pthread_mutex_unlock(data->write);
// 		pthread_mutex_lock(data->meal);
// 		philo->t_lasteat = get_time();
// 		philo->eaten_nb += 1;
// 		usleep(data->t_eat * 1000);
// 		pthread_mutex_unlock(data->meal);
// 		pthread_mutex_unlock(&data->forks[philo->right_fork]);
// 		pthread_mutex_unlock(&data->forks[philo->left_fork]);
// 		pthread_mutex_lock(data->write);
// 		printf("%lld %d is sleeping\n", get_time() - data->start_time, philo->right_fork + 1);
// 		pthread_mutex_unlock(data->write);
// 		usleep(data->t_slp * 1000);
// 	}
// 	return (NULL);
// }