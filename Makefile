# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ego <ego@student.42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/15 12:42:34 by hcavet            #+#    #+#              #
#    Updated: 2025/02/11 18:25:56 by ego              ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

IDIR	=	include/
SDIR	=	src/

SRC		=	pipex.c			\
			string.c		\
			string_utils.c	\
			split.c			\
			data.c			\
			display.c		\
			utils.c			\
			parsing.c		\
			get_next_line.c
SRCS	=	$(addprefix $(SDIR), $(SRC))
OBJS	=	$(SRCS:.c=.o)

CC		=	gcc
RM		=	rm -f
CFLAGS	=	-Wall -Wextra -Werror
NAME	=	pipex

all		:	$(NAME)

$(NAME)	:	$(OBJS) header
			echo "Linking object files..."
			$(CC) $(CFLAGS) $(OBJS) -I $(IDIR) -o $(NAME) 
			echo "$(GREEN)[OK] $(NAME) is ready!$(RESET)"

bonus	:	all

.c.o	:
			echo "Compiling $<..."
			$(CC) $(CFLAGS) -I $(IDIR) -c $< -o $(<:.c=.o)

norm	:
			norminette -R CheckForbiddenSourceHeader $(SRCS) $(IDIR)

clean	:
			echo "Removing object files..."
			$(RM) $(OBJS)
			echo "$(ORANGE)[OK] All object files have been removed.$(RESET)"

fclean	:	clean
			echo "Removing binary files..."
			$(RM) $(NAME)
			echo "$(ORANGE)[OK] All binary files have been removed.$(RESET)"

re		:	fclean all

.PHONY	:	all clean fclean re
.SILENT	:	all $(NAME) bonus $(OBJS) norm clean fclean re header

RED		=	\033[31m
ORANGE	=	\033[38;5;214m
YELLOW	=	\033[33m
GREEN	=	\033[32m
BLUE	=	\033[34m
VIOLET	=	\033[38;5;93m
ITALIC	=	\033[3m
RESET	=	\033[0m

header	:
			echo "============================"
			echo "$(RED) ____  _  ____  ________  __$(RESET)"
			echo "$(RED)/  __\/ \/  __\/  __/\  \/ /$(RESET)"
			echo "$(YELLOW)|  \/|| ||  \/||  \   \   /$(RESET)"
			echo "$(GREEN)|  __/| ||  __/|  /_  /   \ $(RESET)"
			echo "$(BLUE)\_/   \_/\_/   \____\/__/\_\ $(RESET)"
			echo "$(ITALIC)$(VIOLET)           by Ego\n$(RESET)"
			echo "============================\n"
