NAME = minishell
CC = cc
FLAGS = -Wall -Werror -Wextra -g3
READLINE_FLAGS = -lreadline
INCLUDES = -I./libft/includes -I./includes
SUPP_FILE = valgrind_readline_leaks_ignore.supp

# directories
SRC_DIR = src
OBJ_DIR = obj
LIBFT_DIR = libft

# src files
SRC =	src/builtins/pwd.c \
		src/env/env.c \
		src/core/minishell_loop.c \
		src/core/print_ascii_art.c

# object files preserving subdirectory structure
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

LIBFT = $(LIBFT_DIR)/libft.a
LIBTEST = $(OBJ_DIR)/libminishell.a

# rules
all: $(NAME)

# build minishell executable
$(NAME): $(OBJ)
	@make -C $(LIBFT_DIR)
	$(CC) $(FLAGS) $(INCLUDES) $(OBJ) $(LIBFT) -o $(NAME) $(READLINE_FLAGS)

# build object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(FLAGS) $(INCLUDES) -c $< -o $@

# build libminishell.a for tests
libminishell.a: $(LIBTEST)

$(LIBTEST): $(filter-out $(OBJ_DIR)/main.o, $(OBJ))
	@mkdir -p $(dir $@)
	ar rcs $@ $^

$(SUPP_FILE):
	@echo "Creating valgrind suppression file for readline library"
	@echo "{" > $(SUPP_FILE)
	@echo "   ignore_libreadline_leaks" >> $(SUPP_FILE)
	@echo "   Memcheck:Leak" >> $(SUPP_FILE)
	@echo "   ..." >> $(SUPP_FILE)
	@echo "   obj:*/libreadline.so.*" >> $(SUPP_FILE)
	@echo "}" >> $(SUPP_FILE)

valgrind: $(NAME) $(SUPP_FILE)
valgrind: $(SUPP_FILE)
	@echo "valgrind rule ready, but no main program yet"
	@echo "Use: cd tests && make valgrind TEST=your_test.c"
# uncomment this rule when you're ready to test on main.c and delete the 2 lines @echo
# valgrind --suppressions=$(SUPP_FILE) --leak-check=full ./$(NAME)

# clean objects
clean:
	@make -C $(LIBFT_DIR) clean
	rm -rf $(OBJ_DIR)

# full clean
fclean: clean
	@make -C $(LIBFT_DIR) fclean
	rm -f $(NAME)
	rm -f $(SUPP_FILE)

# rebuild all
re: fclean all

.PHONY: all clean fclean re valgrind
