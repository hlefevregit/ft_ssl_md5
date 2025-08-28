# ========== CONFIGURATION ==========
NAME        := ft_ssl
CC          := gcc
CFLAGS      := -Wall -Wextra -Werror

# Add your .c files here
SRCS        := ./src/main.c \
			./src/parser.c \
			./src/utils.c \
			./src/reader.c \
			./src/md5/hash.c \
			./src/md5/padding.c \
			./src/md5/result.c \
			./src/sha256/hash.c \
			./src/sha256/padding.c \
			./src/sha256/result.c \

OBJS        := $(SRCS:.c=.o)

# ========== RULES ==========

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)
	printf "$(GREEN)$(NAME) compiled successfully!$(RESET) 💾\n"
	

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)
	printf "$(YELLOW)Object files removed!$(RESET) 🗑️\n"

fclean: clean
	rm -f $(NAME)
	printf "$(RED)$(NAME) removed!$(RESET) 🗑️\n"

re: fclean all

# Optional: silence output
.SILENT:

.PHONY: all clean fclean re

RED = \033[0;31m
GREEN = \033[0;32m
YELLOW = \033[0;33m
BLUE = \033[0;34m
MAGENTA = \033[0;35m
CYAN = \033[0;36m
WHITE = \033[0;37m
RESET = \033[0m