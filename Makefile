# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/08/27 12:07:56 by copireyr          #+#    #+#              #
#    Updated: 2024/10/07 12:15:08 by copireyr         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.DEFAULT_GOAL := all
CC := cc

CPPFLAGS := -I./include/ -I./libft/include
LDFLAGS := -lreadline -L./libft/ -lft
CFLAGS := -Wall -Wextra -MMD -MP -g3 -Og
asan := -fsanitize=address,undefined#,leak
CFLAGS += $(asan)
LDFLAGS += $(asan)
NAME := minishell

ifeq ($(shell uname -s), Darwin)
	include macOS.mk
endif

src := ./src/main.c ./src/environment.c ./src/signals.c ./src/execute_ast.c

parse := $(addprefix ./src/parse/, ast.c parse.c tokenize.c tokenize_utils.c expand.c)
builtins := $(addprefix ./src/builtins/, builtin_env.c builtin_export.c builtin_unset.c)
src += $(parse) $(builtins)

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
	$(RM) -r ./obj
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
