CFILES = src/philo.c \
		src/helper.c \
		src/checker.c \
		src/setup.c \
		src/watcher.c \

NAME = philo
CC = cc
CFLAGS = -Wall -Wextra -Werror -I./include -g
OBJ_DIR = Obj
OFILES = $(addprefix $(OBJ_DIR)/,$(notdir $(CFILES:.c=.o)))

$(NAME): $(OBJ_DIR) $(OFILES)
	@$(CC) $(OFILES) -o $(NAME)
	clear
	@$(MAKE)	loading
	clear
	@echo "\033[32mphilos are ready to eat\033[0m"

$(OBJ_DIR)/%.o: src/%.c
	@$(CC) -c $(CFLAGS) $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

loading:
		@for i in {1..42}; do \
			printf '%s' "█"; \
			sleep 0.01; \
		done

all: $(NAME)

clean:
	@rm -rf $(OBJ_DIR)
	@rm -f $(OFILES)
	@echo "\033[33mclean\033[0m"

fclean: clean
	@rm -f $(NAME)
	@echo "\033[33mand\033[0m"
	@echo "\033[33mfclean\033[0m"

re: fclean all

.PHONY: all, clean, fclean, re, loading
