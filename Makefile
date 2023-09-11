# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tmoutinh <tmoutinh@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/08/16 01:39:55 by tmoutinh          #+#    #+#              #
#    Updated: 2023/09/11 13:16:08 by tmoutinh         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo
SRCS = srcs/Philosophers.c srcs/routine.c srcs/utils.c srcs/watcher.c srcs/initializer.c

FLAGS = -Wall -Werror -Wextra -g -fsanitize=thread

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