# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/08/27 12:07:56 by copireyr          #+#    #+#              #
#    Updated: 2024/09/25 10:38:05 by pleander         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.DEFAULT_GOAL := all
CC := cc
CPPFLAGS := -I./include/ -I./libft/include
LDFLAGS := -lreadline -L./libft/ -lft
#CFLAGS := -Wall -Wextra -Werror -MMD -MP -g3 -Og
CFLAGS := -Wall -Wextra -Werror -g3
NAME := minishell

src = ./src/main.c ./src/environment.c ./src/builtin_env.c \
	  ./src/tokenize.c ./src/builtin_export.c ./src/builtin_unset.c
obj := $(src:./src/%.c=./obj/%.o)
lib := ./libft/libft.a

./obj/%.o: ./src/%.c
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(lib):
	make -j4 -C ./libft/

$(NAME): $(lib) $(obj)
	$(CC) $^ -o $@ $(LDFLAGS)

.PHONY: all
all: $(NAME)

.PHONY: clean
clean:
	$(RM) -r $(obj)
	make -C ./libft/ clean

.PHONY: fclean
fclean: clean
	$(RM) $(NAME)
	make -C ./libft/ fclean

.PHONY: re
re: fclean all

.PHONY: run
run: $(NAME)
	./$(NAME)

.PHONY: val
val: $(NAME)
	valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes --suppressions=valgrind_readline_suppressions.supp ./$(NAME) < tests/test.txt

-include $(obj:.o=.d)
