NAME		:=	ft_traceroute
NAME_BONUS	:=	$(NAME)_bonus

CC		:=	gcc
CFLAGS	:=	-Wall -Werror -Wextra #-fsanitize=address -g3

SRCS_COMMON		:=	srcs/main.c			\
					srcs/pars.c			\
					srcs/utils.c		\
					srcs/socket.c		\
					srcs/signal.c		\
					srcs/memory.c		\
					srcs/network.c		\
					srcs/i_o_pck.c
SRCS_MANDATORY	:=	$(SRCS_COMMON)		\
					srcs/traceroute.c	\
					srcs/print.c

SRCS_BONUS		:=	$(SRCS_COMMON)			\
					srcs/traceroute_bonus.c	\
					srcs/print_bonus.c

OBJS_MANDATORY	:=	$(SRCS_MANDATORY:.c=.o)
OBJS_BONUS		:=	$(SRCS_BONUS:.c=.o)

all:	$(NAME)

$(NAME):	inc/includes.h $(OBJS_MANDATORY)
	$(CC) $(CFLAGS) $(OBJS_MANDATORY) -o $(NAME)

bonus: $(NAME_BONUS)

$(NAME_BONUS):	inc/includes.h $(OBJS_BONUS)
	$(CC) $(CFLAGS) $(OBJS_BONUS) -o $(NAME_BONUS)


%.o:	%.c inc/includes.h
	${CC} ${CFLAGS} -c $< -o $@ 

clean:
	rm -rf $(OBJS_MANDATORY) $(OBJS_BONUS)

fclean: clean
	rm -rf $(NAME) $(NAME_BONUS)

re: fclean all
