# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pafroidu <pafroidu@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/15 15:16:47 by diade-so          #+#    #+#              #
#    Updated: 2025/09/09 10:19:52 by diade-so         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a
CC = cc
FLAGS = -Wall -Werror -Wextra
INCLUDES = -Iincludes

# Directories
SRC_DIR = src
OBJ_DIR = obj

# Source Files
SRC_CHAR = \
	src/char/ft_isalnum.c \
	src/char/ft_isalpha.c \
	src/char/ft_isascii.c \
	src/char/ft_isdigit.c \
	src/char/ft_isprint.c \
	src/char/ft_isspace.c \
	src/char/ft_tolower.c \
	src/char/ft_toupper.c

SRC_CONV = \
	src/conv/ft_itoa.c \
	src/conv/ft_safe_atoi.c \
	src/conv/ft_atosize.c \


SRC_IO = \
	src/io/ft_putchar_fd.c \
	src/io/ft_putendl_fd.c \
	src/io/ft_putnbr_fd.c \
	src/io/ft_putstr_fd.c

SRC_LIST = \
	src/list/ft_lstadd_back.c \
	src/list/ft_lstadd_front.c \
	src/list/ft_lstclear.c \
	src/list/ft_lstdelone.c \
	src/list/ft_lstiter.c \
	src/list/ft_lstlast.c \
	src/list/ft_lstmap.c \
	src/list/ft_lstnew.c \
	src/list/ft_lstsize.c

SRC_MEMORY = \
	src/memory/ft_bzero.c \
	src/memory/ft_calloc.c \
	src/memory/ft_memchr.c \
	src/memory/ft_memcmp.c \
	src/memory/ft_memcpy.c \
	src/memory/ft_memmove.c \
	src/memory/ft_memset.c

SRC_PRINTF = \
	src/print/ft_printf/ft_printf.c \
	src/print/ft_printf/ft_put.c \
	src/print/ft_printf/pointer.c

SRC_STRING = \
	src/string/ft_split.c \
	src/string/ft_strchr.c \
	src/string/ft_strcmp.c \
	src/string/ft_strdup.c \
	src/string/ft_striteri.c \
	src/string/ft_strjoin.c \
	src/string/ft_strlcat.c \
	src/string/ft_strlcpy.c \
	src/string/ft_strlen.c \
	src/string/ft_strmapi.c \
	src/string/ft_strncmp.c \
	src/string/ft_strnstr.c \
	src/string/ft_strrchr.c \
	src/string/ft_strtrim.c \
	src/string/ft_substr.c \

SRC_READ = \
	src/read/get_next_line.c

SRC = $(SRC_CHAR) $(SRC_CONV) $(SRC_IO) $(SRC_LIST) $(SRC_MEMORY) $(SRC_PRINTF)\
	$(SRC_STRING) $(SRC_READ)

# Object files
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# **************************************************************************** #
#                                   Rules                                      #
# **************************************************************************** #

all: $(NAME)

$(NAME): $(OBJ)
	ar rcs $(NAME) $(OBJ)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(FLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
