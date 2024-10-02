# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/08/27 12:07:56 by copireyr          #+#    #+#              #
#    Updated: 2024/10/01 16:27:01 by pleander         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.DEFAULT_GOAL := all
CC := cc
CPPFLAGS := -I./include/ -I./libft/include
LDFLAGS := -lreadline -L./libft/ -lft
CFLAGS := -Wall -Wextra -MMD -MP -g3 -Og
asan := -fsanitize=address,undefined,leak
CFLAGS += $(asan)
LDFLAGS += $(asan)
NAME := minishell

src = ./src/main.c ./src/environment.c ./src/builtin_env.c \
	  ./src/tokenize.c ./src/tokenize_utils.c \
	  ./src/builtin_export.c ./src/builtin_unset.c \
	  ./src/ast.c ./src/parse.c ./src/signals.c

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
