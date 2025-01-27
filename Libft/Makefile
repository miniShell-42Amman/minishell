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
		ft_memmove.c ft_memset.c					\
		ft_split.c ft_strchr.c						\
		ft_strdup.c ft_strjoin.c					\
		ft_strlcat.c ft_strlcpy.c					\
		ft_strlen.c ft_strmapi.c					\
		ft_strncmp.c ft_strnstr.c					\
		ft_strrchr.c ft_strtrim.c					\
		ft_substr.c ft_tolower.c					\
		ft_toupper.c ft_putnbr_fd.c					\
		ft_striteri.c ft_atol.c						\
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
		printf/ft_printf_hex_uppercase.c			\
		printf/ft_printf_char.c						\
		get_next_line/get_next_line.c				\
		get_next_line/get_next_line_bonus.c			\
		dprintf/ft_dprintf.c					\
		dprintf/ft_dprintf_unsigned.c			\
		dprintf/ft_dprintf_string.c				\
		dprintf/ft_dprintf_ptr.c				\
		dprintf/ft_dprintf_number.c				\
		dprintf/ft_dprintf_hex.c				\
		dprintf/ft_dprintf_hex_uppercase.c		\
		dprintf/ft_dprintf_char.c				\
		dprintf/ft_utils.c						\

OBJS = $(SRC:%.c=$(OBJDIR)/%.o)

all: $(NAME)

$(OBJDIR):
	mkdir -p $(OBJDIR)
	mkdir -p $(OBJDIR)/printf
	mkdir -p $(OBJDIR)/dprintf
	mkdir -p $(OBJDIR)/get_next_line

$(OBJDIR)/%.o: %.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	$(AR) $(NAME) $(OBJS)

clean:
	$(RM) $(OBJS)
	$(RM) -r $(OBJDIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re