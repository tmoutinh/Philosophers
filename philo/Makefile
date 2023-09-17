# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tmoutinh <tmoutinh@student.42porto.com     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/08/16 01:39:55 by tmoutinh          #+#    #+#              #
#    Updated: 2023/09/17 18:22:09 by tmoutinh         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo
SRCS = srcs/Philosophers.c srcs/routine.c srcs/utils.c srcs/watcher.c srcs/initializer.c

FLAGS = -Wall -Werror -Wextra -pthread -g

CC = cc

SRCS_DIR = srcs

RM = rm -f

OBJS = $(SRCS:.c=.o)

$(SRCS_DIR)/%.o: $(SRCS_DIR)/%.c
		${CC} ${FLAGS} -c $< -o $@

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(FLAGS) $(SRCS) -o $(NAME)

clean:
	$(RM) $(OBJS)

fclean: clean
	${RM} ${NAME}

re: fclean all

.PHONY:all clean fclean re