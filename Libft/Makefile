CC = cc 
CFLAGS = -Wall -Wextra -Werror -I.
AR = ar rcs
RM = rm -f
NAME = libft.a
OBJDIR = obj

SRC =	ft_atoi.c ft_bzero.c						\
		ft_calloc.c ft_putendl_fd.c					\
		ft_itoa.c ft_putchar_fd.c					\
		ft_putstr_fd.c 	ft_atol_handle.c			\
		ft_memchr.c ft_memcmp.c						\
		ft_memcpy.c ft_isalnum.c					\
		ft_isalpha.c ft_isascii.c					\
		ft_isdigit.c ft_isprint.c					\
		ft_isspace.c 								\
		ft_memmove.c ft_memset.c					\
		ft_split.c ft_strchr.c						\
		ft_strdup.c ft_strjoin.c					\
		ft_strlcat.c ft_strlcpy.c ft_strcmp.c		\
		ft_strlen.c ft_strmapi.c					\
		ft_strncmp.c ft_strnstr.c					\
		ft_strrchr.c ft_strtrim.c					\
		ft_substr.c ft_tolower.c					\
		ft_toupper.c ft_putnbr_fd.c					\
		ft_striteri.c ft_atol.c						\
		ft_strlen_sep.c								\
		ft_realloc.c 							    \
		ft_lstadd_back_bonus.c						\
		ft_lstadd_front_bonus.c						\
		ft_lstclear_bonus.c							\
		ft_lstdelone_bonus.c						\
		ft_lstiter_bonus.c							\
		ft_lstlast_bonus.c							\
		ft_lstsize_bonus.c							\
		ft_lstnew_bonus.c							\
		ft_lstmap_bonus.c							\
		printf/ft_printf.c							\
		printf/ft_printf_unsigned.c					\
		printf/ft_printf_string.c					\
		printf/ft_printf_ptr.c						\
		printf/ft_printf_number.c 					\
		printf/ft_printf_hex.c						\
		printf/ft_printf_uhex.c						\
		printf/ft_printf_char.c						\
		get_next_line/get_next_line.c				\
		get_next_line/g_n_l.c						\
		dprintf/ft_dprintf.c						\
		dprintf/ft_dprintf_unsigned.c				\
		dprintf/ft_dprintf_string.c					\
		dprintf/ft_dprintf_ptr.c					\
		dprintf/ft_dprintf_number.c					\
		dprintf/ft_dprintf_hex.c					\
		dprintf/ft_dprintf_uhex.c					\
		dprintf/ft_dprintf_char.c					\
		dprintf/ft_utils.c							\

OBJS = $(SRC:%.c=$(OBJDIR)/%.o)

N		=	\033[m
R		=	\033[0;31m
G		=	\033[0;32m
Y		=	\033[0;33m
B		=	\033[0;34m
P		=	\033[0;35m
T		=	\033[0;36m

all: $(NAME)
	@printf "$(B)%b $(Y)%b $(B)%b$(N)\n" "###############################" "$(NAME) ready" "###"


$(OBJDIR):
	@mkdir -p $(OBJDIR)
	@mkdir -p $(OBJDIR)/printf
	@mkdir -p $(OBJDIR)/dprintf
	@mkdir -p $(OBJDIR)/get_next_line

$(OBJDIR)/%.o: %.c | $(OBJDIR)
	@$(CC) $(CFLAGS) -c $< -o $@
	@printf "$(B)%b $(Y)%10b $(T)%-34b $(B)%b$(N)\n" "#" "compiling" "$@" "#"

$(NAME): $(OBJS)
	@$(AR) $(NAME) $(OBJS)

header:
	@printf "$(B)%b $(Y)%b $(B)%b$(N)\n" "###################" "L I B F T" "###################"

clean: header
	@$(RM) -r $(OBJS)
	@$(RM) -r $(OBJDIR)
	@printf "$(B)%b $(Y)%10b $(T)%-34b $(B)%b$(N)\n" "#" "removing" "objects" "#"
	@printf "$(B)%b $(Y)%b $(B)%b$(N)\n" "##################################" "clean done" "###"

fclean: clean
	@$(RM) -r $(NAME)
	@printf "$(B)%b $(Y)%10b $(T)%-34b $(B)%b$(N)\n" "#" "removing" "$(NAME)" "#"
	@printf "$(B)%b $(Y)%b $(B)%b$(N)\n" "#################################" "fclean done" "###"

re: fclean all

.PHONY: all clean fclean re