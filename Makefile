files = philo.c \

NAME = philo
CC = cc
CFLAGS = -Wall -Wextra -Werror
OFILES = $(files:.c=.o)

$(NAME): $(OFILES)
	ar rsc $(NAME) $(OFILES)

%.o: %.c
	$(CC) -c $(CFLAGS) $?

loading:
		@for i in {1..42}; do \
			printf '%s' "█"; \
			sleep 0.01; \
		done

all: $(NAME)
	clear;
	# @$(MAKE)loading
	clear;

clean:
	rm -f $(OFILES)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all, clean, fclean, re, loading
