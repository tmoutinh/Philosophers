# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tmoutinh <tmoutinh@student.42porto.com     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/08/16 01:39:55 by tmoutinh          #+#    #+#              #
#    Updated: 2023/08/18 23:13:41 by tmoutinh         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

all:
	cc -g -Wall -Werror -Wextra -lpthread Philosophers.c utils.c -o philo_one
	cp philo_one ../philosophers/philo_one/philo_one