CFILES = src/philo.c \
		src/helper.c \
		src/checker.c \
		src/setup.c \
		src/watcher.c \
		src/cleanup.c \

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
	@echo "\033[32mUsage: ./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]\033[0m"
	@echo
	@echo "\033[0;31mArguments:\033[0m"
	@echo "  \033[0;31mnumber_of_philosophers:\033[0m The number of philosophers (1-200)"
	@echo "  \033[0;31mtime_to_die:\033[0m Time in milliseconds until a philosopher dies from starvation"
	@echo "  \033[0;31mtime_to_eat:\033[0m Time in milliseconds it takes for a philosopher to eat"
	@echo "  \033[0;31mtime_to_sleep:\033[0m Time in milliseconds a philosopher spends sleeping"
	@echo "  \033[0;31mnumber_of_times_each_philosopher_must_eat:\033[0m (Optional) Simulation stops after all philosophers eat this many times"
	@echo
	@echo "\033[0;33mExample:\033[0m"
	@echo "\033[0;33m  ./philo 5 800 200 200 7\033[0m"

$(OBJ_DIR)/%.o: src/%.c
	@$(CC) -c $(CFLAGS) $< -o $@

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

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
