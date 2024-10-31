# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/08/27 12:07:56 by copireyr          #+#    #+#              #
#    Updated: 2024/10/17 09:51:37 by copireyr         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.DEFAULT_GOAL := all
CC := cc

CPPFLAGS := -I./include/ -I./libft/include
LDFLAGS := -lreadline -L./libft/ -lft
CFLAGS := -Wall -Wextra -MMD -MP -g3
asan := -fsanitize=address,undefined#,leak
CFLAGS += $(asan)
LDFLAGS += $(asan)
NAME := minishell

ifeq ($(shell uname -s), Darwin)
	include macOS.mk
endif

src := ./src/main.c ./src/environment.c ./src/signals.c

parse := $(addprefix ./src/parse/, ast.c parse.c tokenize.c tokenize_utils.c \
		 expand.c glob.c split_words.c remove_quotes.c glob_quotes.c)
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
re:
	@make -s fclean
	@make -s -j

.PHONY: run
run: $(NAME)
	./$(NAME)

.PHONY: val
val: $(NAME)
	valgrind --suppressions=readline.supp -s ./$(NAME)


.PHONY: test
test: $(NAME)
	# SPACE_VAR="hello world" EMPTY="" ./$< < tests/expansion.msh
	# ./$< < tests/glob.msh
	./$< < tests/quotes.msh
-include $(obj:.o=.d)
