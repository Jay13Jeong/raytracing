NAME		= miniRT
CC			= gcc
CFLAGS		= -Wall -Werror -Wextra
RM			= rm -f
GNL_DIR		= ./get_next_line/
LIBFT_DIR	= ./libft/
MLX_DIR		= ./mlx/
INCS_DIR	= ./includes/
SRCS_DIR	= ./srcs/
SRC			= hit.c \
			  hit_cylinder.c \
			  hit_plane.c \
			  hit_sphere.c \
			  init.c \
			  main.c \
			  parse.c \
			  parse_figures.c \
			  phong_lighting.c \
			  ray.c \
			  utils.c \
			  vec_operation.c \
			  vec_utils.c \
			  a2f.c
SRCS		= $(addprefix $(SRCS_DIR), $(SRC))
OBJS		= $(SRCS:.c=.o)

.c.o :
	$(CC) $(CFLAGS) -I $(INCS_DIR) -o $@ -c $?

$(NAME) : $(OBJS)
	make -C $(LIBFT_DIR)
	make -C $(GNL_DIR)
	make -C $(MLX_DIR)
	$(CC) -o $(NAME) $(OBJS) $(GNL_DIR)get_next_line.a -I $(INCS_DIR) -L $(LIBFT_DIR) -lft -L $(MLX_DIR) -lmlx -framework OpenGL -framework Appkit

all : $(NAME)

bonus : all

clean : 
	make -C $(LIBFT_DIR) clean
	make -C $(GNL_DIR) clean
	make -C $(MLX_DIR) clean
	$(RM) $(OBJS)

fclean : clean
	make -C $(LIBFT_DIR) fclean
	make -C $(GNL_DIR) fclean
	$(RM) $(NAME)

re : clean all

.PHONY : all clean fclean re
