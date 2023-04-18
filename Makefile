NAME	:=	ft_traceroute

CC		:=	gcc
CFLAGS	:=	-Wall -Werror -Wextra -fsanitize=address -g3 -DBONUS

SRCS	:=	srcs/main.c			\
			srcs/pars.c			\
			srcs/utils.c		\
			srcs/socket.c		\
			srcs/traceroute.c	\
			srcs/print.c		\
			srcs/signal.c		\
			srcs/memory.c		\
			srcs/network.c		\
			srcs/i_o_pck.c		\

OBJS	:=	$(SRCS:.c=.o)

all:	$(NAME)

$(NAME):	inc/includes.h $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o:	%.c inc/includes.h
	${CC} ${CFLAGS} -c $< -o $@ 

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all
