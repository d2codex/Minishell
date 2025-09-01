CC = cc
FLAGS = -Wall -Wextra -Werror -g3
INCLUDES = -I../includes -I../libft/includes

# Directories
UNIT_DIR = unit
INTEG_DIR = integration
BIN_DIR = bin

# Libraries
LIBFT = ../libft/libft.a
LIBMS = ../obj/libminishell.a
LIBS = $(LIBMS) $(LIBFT)

# Find all test files
UNIT_TESTS := $(wildcard $(UNIT_DIR)/test_*.c)
INTEG_TESTS := $(wildcard $(INTEG_DIR)/test_*.c)

# Binaries
UNIT_BIN := $(UNIT_TESTS:%.c=$(BIN_DIR)/%)
INTEG_BIN := $(INTEG_TESTS:%.c=$(BIN_DIR)/%)

# Default target: build and run all tests
all: $(UNIT_BIN) $(INTEG_BIN)
	@echo "\nRunning all tests..."
	@for t in $(UNIT_BIN) $(INTEG_BIN); do \
		./$$t || echo "FAILED $$t"; \
	done

# Rule to build unit test binaries
$(BIN_DIR)/%: %.c $(LIBS)
	@mkdir -p $(dir $@)
	$(CC) $(FLAGS) $(INCLUDES) $< $(LIBS) -o $@

# Build dependent libraries without building main executable
$(LIBFT):
	$(MAKE) -C ../libft

$(LIBMS):
	$(MAKE) -C .. libminishell.a

# Clean test binaries
clean:
	rm -rf $(BIN_DIR)

# Full clean including libraries
fclean: clean
	$(MAKE) -C .. fclean
	$(MAKE) -C .. clean

# Rebuild all tests
re: fclean all

# --- Single test helpers ---

# Build a single test binary
# Usage: make build TEST=unit/test_name.c
# Execute from root: ./test.test_name.c
build: $(BIN_DIR)/$(basename $(notdir $(TEST)))

$(BIN_DIR)/$(basename $(notdir $(TEST))): $(TEST) $(LIBS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(FLAGS) $(INCLUDES) $(TEST) $(LIBS) -o $@

# Optional: run a single test
# Usage: make run TEST=unit/test_name.c
run: build
	./$(BIN_DIR)/$(basename $(notdir $(TEST)))

.PHONY: all clean fclean re run
