# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/08/27 12:07:56 by copireyr          #+#    #+#              #
#    Updated: 2024/10/30 11:29:24 by pleander         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.DEFAULT_GOAL := all
CC := cc

DEBUG := 0
CPPFLAGS := -I./include/ -I./libft/include
LDFLAGS := -lreadline -L./libft/ -lft
CFLAGS := -Wall -Wextra -MMD -MP -g
asan := -fsanitize=address,undefined,leak
CFLAGS += $(asan)
LDFLAGS += $(asan)
NAME := minishell

ifeq ($(shell uname -s), Darwin)
	include macOS.mk
endif

src := ./src/main.c ./src/environment.c ./src/signals.c

parse := $(addprefix ./src/parse/, ast.c parse.c tokenize.c tokenize_utils.c expand.c)
builtins := $(addprefix ./src/builtins/, builtins.c builtin_env.c builtin_export.c builtin_unset.c builtin_pwd.c builtin_cd.c builtin_echo.c)
execute := $(addprefix ./src/execute/, execute_ast.c make_command.c pipe.c run_builtin.c)
src += $(parse) $(builtins) $(execute)

obj := $(src:./src/%.c=./obj/%.o)
lib := ./libft/libft.a

./obj/%.o: ./src/%.c
	@mkdir -p $(@D)
	$(CC) -D DEBUG=$(DEBUG) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

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
re:
	@make -s fclean
	@make -s -j

.PHONY: run
run: $(NAME)
	./$(NAME)

.PHONY: val
val: $(NAME)
	valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes --suppressions=valgrind_readline_suppressions.supp ./$(NAME) < tests/test.txt


.PHONY: test
test: $(NAME)
	# SPACE_VAR="hello world" EMPTY="" ./$< < tests/expansion.msh
	./$< < tests/glob.msh
-include $(obj:.o=.d)
