# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/08/27 12:07:56 by copireyr          #+#    #+#              #
#    Updated: 2024/08/28 12:16:28 by copireyr         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.DEFAULT_GOAL := all
CC := cc
CPPFLAGS := -I./include/
LDFLAGS := -lreadline
CFLAGS := -Wall -Wextra -Werror -MMD -MP
NAME := minishell

src = ./src/main.c
obj := $(src:./src/%.c=./obj/%.o)

./obj/%.o: ./src/%.c
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(NAME): $(obj)
	$(CC) $^ -o $@ $(LDFLAGS)

.PHONY: all
all: $(NAME)

.PHONY: clean
clean:
	$(RM) -r $(obj)

.PHONY: fclean
fclean: clean
	$(RM) $(NAME)

.PHONY: re
re: fclean all

.PHONY: run
run: $(NAME)
	./$^

-include $(obj:.o=.d)
