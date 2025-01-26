CC = cc
CFLAGE = -Wall -Wextra -Werror -I.
RM = rm -r
NAME = minishell
LIBFT_DIR = ./Libft
LIBFT = $(LIBFT_DIR)/libft.a

SRC = src/main.c

OBJS = $(SRC:.c=.o)

all: $(LIBFT) $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGE) -o $(NAME) $(OBJS) $(LIBFT) -lreadline 

clean: 
	$(RM) $(OBJS)
	$(MAKE) -C $(LIBFT_DIR) clean


fclean: clean
	$(RM) $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re