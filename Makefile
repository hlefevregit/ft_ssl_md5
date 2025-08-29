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
			./src/base64/encoder.c \
			./src/base64/process.c \
			./src/flags.c \
			./src/des/args_process.c \
			./src/des/process.c \
			./src/des/ecb/process.c \
			./src/des/ecb/padding.c \
			./src/des/ecb/encrypter.c \
			./src/des/ecb/schedule.c \
			# ./src/des/cbc/process.c \


OBJS        := $(SRCS:.c=.o)

# ========== RULES ==========

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

# Optional: silence output
.SILENT:

.PHONY: all clean fclean re