# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ego <ego@student.42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/15 12:42:34 by hcavet            #+#    #+#              #
#    Updated: 2025/02/04 20:24:18 by ego              ###   ########.fr        #
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

$(BNAME):	bonus

bonus	:	all

.c.o	:
			echo "Compiling $<..."
			$(CC) $(CFLAGS) -I $(IDIR) -c $< -o $(<:.c=.o)

norm	:
			norminette -R CheckForbiddenSourceHeader $(SRCS) $(IDIR)

clean	:
			echo "Removing object files..."
			$(RM) $(OBJS) $(LOG)
			echo "$(ORANGE)[OK] All object files have been removed.$(RESET)"

fclean	:	clean
			echo "Removing binary files..."
			$(RM) $(NAME) $(BNAME)
			echo "$(ORANGE)[OK] All binary files have been removed.$(RESET)"

VGRIND	=	valgrind
VFLAGS	=	--leak-check=full
TEST	=	make --no-print-directory test
LOG		=	log
MIN		=	-1000
OS		=	linux
MAX		=	1000
N		=	5

test	:
			echo "$(ORANGE)Testing for n = $(N)$(RESET)"
			echo "N = $(N) TEST" >> $(LOG)
			$(eval ARG = $(shell seq $(MIN) $(MAX) | shuf -n $(N)))
			if [ "$(N)" -le 10 ]; then echo "Initial stack:\t$(ARG)"; fi
			echo -n "Checker:\t"
			$(eval CHECK = $(shell ./push_swap $(ARG) | ./checker_$(OS) $(ARG)))
			if [ "$(CHECK)" = "OK" ]; then echo -n "$(GREEN)"; else echo -n "$(RED)"; fi
			echo "$(CHECK)$(RESET)"
			echo -n "Moves:\t\t"
			./push_swap $(ARG) | wc -l
			$(VGRIND) $(VFLAGS) ./$(NAME) $(ARG) >> $(LOG) 2>&1
			echo "" >> $(LOG)
			echo "$(BLUE)Moves and leak check stored in $(LOG) file.$(RESET)"
			if [ $(N) -le 499 ]; then echo ""; fi

leaks	:
			$(eval DEF = $(shell awk '/definitely lost:/ {sum += $$4} END {if(sum) print sum; else print "0"}' $(LOG)))
			$(eval IND = $(shell awk '/indirectly lost:/ {sum += $$4} END {if(sum) print sum; else print "0"}' $(LOG)))
			$(eval POS = $(shell awk '/possibly lost:/ {sum += $$4} END {if(sum) print sum; else print "0"}' $(LOG)))
			$(eval TOT = $(shell awk '/lost:/ {sum += $$4} END {if(sum) print sum; else print "0"}' $(LOG)))
			$(eval CDEF = $(if $(shell [ $(DEF) -ge 1 ]), $(RED), $(GREEN)))
			$(eval CIND = $(if $(shell [ $(IND) -ge 1 ]), $(RED), $(GREEN)))
			$(eval CPOS = $(if $(shell [ $(POS) -ge 1 ]), $(RED), $(GREEN)))
			$(eval CTOT = $(if $(shell [ $(TOT) -ge 1 ]), $(RED), $(GREEN)))
			echo "Definitely lost:\t$(CDEF)$(DEF)$(RESET) bytes"
			echo "Indirectely lost:\t$(CIND)$(IND)$(RESET) bytes"
			echo "Possibly lost:\t\t$(CPOS)$(POS)$(RESET) bytes"
			echo "Total:\t\t\t$(CTOT)$(TOT)$(RESET) bytes"
			if [ $(TOT) -ge 1 ]; then echo "$(RED)[KO] Leaks!$(RESET)"; else echo "$(GREEN)[OK] No leaks!$(RESET)"; fi

re		:	fclean all

.PHONY	:	all clean fclean re
.SILENT	:	all $(NAME) $(OBJS) $(BOBJS) norm debug clean fclean re header test bonus tests leaks btest btests

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
