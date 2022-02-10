NAME	=   get_next_line.a
SRCS	=	get_next_line.c get_next_line_utils.c
OBJS	= $(SRCS:.c=.o)

all: $(NAME)

.c.o :
	gcc -Wall -Wextra -Werror -c $< -o ${<:.c=.o} -I.

$(NAME): ${OBJS}
	ar -rcs $(NAME) $(OBJS)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)
    
re: fclean all

.PHONY: all .c.o clean fclean re