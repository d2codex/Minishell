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
SRC =	src/env/env_import.c \
		src/core/minishell_loop.c \
		src/core/execute_tokenizer.c \
		src/core/execute_builtins.c \
		src/core/init_shell.c \
		src/core/print_ascii_art.c \
		src/builtins/env.c \
		src/builtins/exit.c \
		src/builtins/export.c \
		src/builtins/export_array.c \
		src/builtins/export_sort.c \
		src/builtins/export_sort_utils.c \
		src/builtins/export_update.c \
		src/builtins/export_utils.c \
		src/builtins/pwd.c \
		src/parser/tokenizer_smart_split.c \
		src/parser/tokenizer_count_tokens.c \
		src/parser/tokenizer_utils.c \
		src/utils/is_whitespace.c \
		src/utils/memory_cleanup.c \
		src/utils/print_error_multi.c \
		src/utils/print_error.c \
		src/main.c

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
	valgrind --suppressions=$(SUPP_FILE) --leak-check=full ./$(NAME)

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
