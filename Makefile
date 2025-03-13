CC = cc
CFLAGS = -Wall -Wextra -Werror -g -Iinclude -I$(LIBFT_DIR)
RM = rm -f
NAME = minishell
LIBFT_DIR = ./Libft
LIBFT = $(LIBFT_DIR)/libft.a

SRC = 	src/main.c 								\
	   	src/env/get_env.c 						\
	   	src/env/expand_env_var_utils.c 			\
		src/free_resources/free_env.c 			\
		src/token/ft_count_token.c 				\
		src/token/ft_count_args.c				\
		src/token/token.c 						\
		src/env/expand_env_var_in_token.c 		\
		src/env/expand_env_utils_second.c 		\
		src/free_resources/free_main.c			\
		src/token/store_token.c 				\
		src/token/token_utils.c			 		\
		src/free_resources/free_parse.c			\
		src/execute/build_env.c					\
		src/execute/utils/ft_strtok.c			\
		src/execute/utils/list_utils.c			\
		src/execute/utils/free_execute.c		\
		src/builtins/echo.c						\
		src/builtins/cd.c						\
		src/builtins/pwd.c						\
		src/builtins/env.c						\
		src/builtins/export.c					\
		src/builtins/unset.c					\
		src/execute/redirections.c				\
		src/builtins/utils/export_utils.c		\
		src/builtins/exit.c						\
		src/signal/handle_signal.c				\
		src/execute/utils/free_redirections.c	\
		src/utils/helper_main.c					\
		src/env/handle_env.c					\
		src/token/disassimble_args.c			\
		src/utils/smart_split.c					\
		src/utils/ft_printf_error.c				\
		src/utils/token_helper.c				\
		src/token/init_parse_cmd.c				\
		src/token/utils_disassimble.c			\
		src/builtins/utils/cd_utils.c			\
		src/utils/expand_utils.c				\
		src/expand/calculate_length.c			\
		src/expand/process_variable.c			\
		src/expand/process_variable_function.c	\
		src/expand/handle_var_length.c			\
		src/env/utils/dollar_fncs.c				\
		src/token/token_utils_second.c			\
		src/execute/utils/utils_execute.c		\
		src/execute/resolve_command_path.c		\
		src/execute/resolve_cmd_path_utils.c	\
		src/execute/pipe_and_fork.c				\
		src/execute/utils/execute_builtin.c		\
		src/execute/full_and_init_cmd.c			\
		src/execute/preprocess_heredocs.c		\
		src/execute/handle_heredoc.c			\
		src/execute/redirections_check.c		\
		src/execute/utils/preprocess_heredocs_utils.c	\

OBJ_DIR = obj
OBJS = $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))

RED = \033[0;31m
RESET = \033[0m
N		=	\033[m
R		=	\033[0;31m
G		=	\033[0;32m
Y		=	\033[0;33m
B		=	\033[0;34m
P		=	\033[0;35m
T		=	\033[0;36m


all: $(LIBFT) $(NAME)

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR) --no-print-directory

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@
	@printf "$(R)%b $(Y)%10b $(G)%-34b $(R)%b$(R)\n" "#" "compiling" "$@"


# -lreadline 
$(NAME): $(LIBFT) $(OBJS)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBFT) -lreadline 
	@echo "$(RED)#################################################$(RESET)"
	@echo "$(RED)#        💀💀💀 minishell ready 💀💀💀          #$(RESET)"
	@echo "$(RED)#      💻💻💻  Credit by Loiy & Omar 💻💻💻	#$(RESET)"
	@echo "$(RED)#################################################$(RESET)"

clean: 
	@$(RM) -r $(OBJ_DIR)
	@$(MAKE) -C $(LIBFT_DIR) clean --no-print-directory

fclean: clean
	@$(RM) $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean --no-print-directory

re: fclean all

.PHONY: all clean fclean re