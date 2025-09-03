NAME = minishell
CC = cc
FLAGS = -Wall -Werror -Wextra -g3
INCLUDES = -I./libft/includes -I./includes 

# directories
SRC_DIR = src
OBJ_DIR = obj
LIBFT_DIR = libft

# src files
SRC = src/builtins/pwd.c \
	  src/env/env.c 

# object files preserving subdirectory structure
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

LIBFT = $(LIBFT_DIR)/libft.a
LIBTEST = $(OBJ_DIR)/libminishell.a

# rules
all: $(NAME)

# build minishell executable
$(NAME): $(OBJ)
	@make -C $(LIBFT_DIR)
	$(CC) $(FLAGS) $(INCLUDES) $(OBJ) $(LIBFT) -o $(NAME)

# build object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(FLAGS) $(INCLUDES) -c $< -o $@

# build libminishell.a for tests
libminishell.a: $(LIBTEST)

$(LIBTEST): $(filter-out $(OBJ_DIR)/main.o, $(OBJ))
	@mkdir -p $(dir $@)
	ar rcs $@ $^

# clean objects
clean:
	@make -C $(LIBFT_DIR) clean
	rm -rf $(OBJ_DIR)

# full clean
fclean: clean
	@make -C $(LIBFT_DIR) fclean
	rm -f $(NAME)

# rebuild all
re: fclean all

.PHONY: all clean fclean re
