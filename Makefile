# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tmoutinh <tmoutinh@student.42porto.com     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/08/16 01:39:55 by tmoutinh          #+#    #+#              #
#    Updated: 2023/08/18 14:33:34 by tmoutinh         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

all:
	cc -g -Wall -Werror -Wextra -lpthread Philosophers.c utils.c -o philo_one