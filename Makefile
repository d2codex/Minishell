NAME = minishell
CC = cc
FLAGS = -Wall -Werror -Wextra -g3
INCLUDES = -I./libft/includes -I./includes

# directories
SRC_DIR = src
OBJ_DIR = obj
LIBFT_DIR = libft

# src files
SRC =	src/builtins/pwd.c \
		src/parser/tokeniser_smart_split.c \
		src/parser/tokeniser_utils.c \

# object files preserving subdirectory structure
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
LIBFT = $(LIBFT_DIR)/libft.a
LIBTEST = $(OBJ_DIR)/libminishell.a

# rules
all: $(NAME)

$(NAME): $(OBJ)
	@make -C $(LIBFT_DIR)
	$(CC) $(FLAGS) $(INCLUDES) $(OBJ) $(LIBFT) -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(FLAGS) $(INCLUDES) -c $< -o $@

libminishell.a: $(LIBTEST)

$(LIBTEST): $(filter-out $(OBJ_DIR)/main.o, $(OBJ))
	ar rcs $@ $^

clean:
	@make -C $(LIBFT_DIR) clean
	rm -rf $(OBJ_DIR)

fclean: clean
	@make -C $(LIBFT_DIR) fclean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
