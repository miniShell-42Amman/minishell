CC = cc
CFLAGS = -Wall -Wextra -Werror -g -Iinclude -I$(LIBFT_DIR)
RM = rm -f
NAME = minishell
LIBFT_DIR = ./Libft
LIBFT = $(LIBFT_DIR)/libft.a

SRC = 	src/main.c 							\
	   	src/env/get_env.c 					\
	   	src/env/expand_env_var_utils.c 		\
		src/free_resources/free_env.c 		\
		src/token/ft_count_token.c 			\
		src/token/ft_count_args.c			\
		src/token/token.c 					\
		src/env/expand_env_var_in_token.c 	\
		src/env/expand_env_utils_second.c 	\
		src/free_resources/free_main.c		\
		src/token/store_token.c 			\
		src/token/token_utils.c			 	\
		src/free_resources/free_parse.c		\
		src/execute/build_env.c				\
		src/execute/utils/ft_strtok.c		\
		src/execute/utils/list_utils.c		\
		src/execute/utils/free_execute.c	\
		src/builtins/echo.c					\
		src/builtins/cd.c					\
		src/builtins/pwd.c					\
		src/builtins/env.c					\
		src/builtins/export.c				\
		src/builtins/unset.c				\
		src/execute/redirections.c			\
		src/builtins/export_utils.c			\


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

# -lreadline 
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