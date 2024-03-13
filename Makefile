# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/01/25 23:32:02 by caguillo          #+#    #+#              #
#    Updated: 2024/03/13 01:54:05 by caguillo         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	pipex
NAME_BONUS	=	pipex_bonus

SRCS		=	./srcs/main.c \
				./srcs/child.c \
				./srcs/ft_split.c \
				./srcs/libft.c \
				./srcs/path.c \
				./srcs/free.c \
				./srcs/get_next_line.c \
				./srcs/get_next_line_utils.c

SRCS_BONUS	=	./srcs/main_bonus.c \
				./srcs/child_bonus.c \
				./srcs/ft_split.c \
				./srcs/libft.c \
				./srcs/path.c \
				./srcs/free.c \
				./srcs/get_next_line.c \
				./srcs/get_next_line_utils.c \
				./srcs/ft_putnbr_fd.c

INCS_SRCS	=	./includes/pipex.h
INCS_DIR	=	-Iincludes

CC			=	gcc
CFLAGS		=	-Wall -Wextra -Werror

OBJS		=	$(SRCS:.c=.o)
OBJS_BONUS	=	$(SRCS_BONUS:.c=.o)

%.o:%.c		$(INCS_SRCS)
			$(CC) $(CFLAGS) $(INCS_DIR) -c $< -o $(<:.c=.o)

$(NAME):	$(OBJS)
			$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
			
all:		$(NAME)

re: 		fclean all

$(NAME_BONUS):	$(OBJS_BONUS) 
			$(CC) $(CFLAGS) $(OBJS_BONUS) -o $(NAME_BONUS)

bonus:		$(NAME_BONUS)

rebonus: 	fclean bonus
			
clean:
			rm -f $(OBJS) $(OBJS_BONUS)

fclean: 	clean
			rm -f $(NAME) $(NAME_BONUS)

.PHONY: 	all clean fclean re bonus rebonus
