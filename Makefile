CC = cc
CFLAGS = -Wall -Wextra -Werror -g -Iinclude -I$(LIBFT_DIR)
RM = rm -f
NAME = minishell
LIBFT_DIR = ./Libft
LIBFT = $(LIBFT_DIR)/libft.a

SRC = src/main.c \
	  src/token/token.c \
	  src/token/store_token.c \
	  src/utils/free.c \
	  src/token/utils_token.c \
	  src/env/get_env.c \

OBJ_DIR = obj
OBJS = $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))

RED = \033[0;31m
RESET = \033[0m

all: $(LIBFT) $(NAME)

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(LIBFT) $(OBJS)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBFT) -lreadline 
	@echo "$(RED)#################################################$(RESET)"
	@echo "$(RED)#        💀💀💀 minishell ready 💀💀💀          #$(RESET)"
	@echo "$(RED)#################################################$(RESET)"

clean: 
	@$(RM) -r $(OBJ_DIR)
	@$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	@$(RM) $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re