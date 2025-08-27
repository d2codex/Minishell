CC = cc
FLAGS = -Wall -Wextra -Werror -g3
INCLUDES = -I../includes -I../libft

# Directories
UNIT_DIR = unit
INTEG_DIR = integration
LIB = ../obj/libminishell.a
BIN_DIR = bin

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
$(BIN_DIR)/%: %.c $(LIB)
	@mkdir -p $(BIN_DIR)
	$(CC) $(FLAGS) $(INCLUDES) $< $(LIB) -o $@

# Build library  from parent Makefile
$(LIB):
	$(MAKE) -C .. libminishell.a

# Clean test binaries
clean:
	rm -rf $(BIN_DIR)

# Full clean including library
fclean: clean
	$(MAKE) -C .. clean

# Rebuild all tests
re: fclean all

# Optional: run a single test
# Usage: make run TEST=unit/test_cd
run: $(LIB)
	$(CC) $(FLAGS) $(INCLUDES) $(TEST).c $(LIB) -o $(BIN_DIR)/$(notdir $(TEST))
	./$(BIN_DIR)/$(notdir $(TEST))

.PHONY: all clean fclean re run
