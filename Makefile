# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/11/30 10:00:57 by dde-jesu          #+#    #+#              #
#    Updated: 2019/05/29 06:01:49 by dde-jesu         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = lem-in

BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/objs
DEP_DIR = $(BUILD_DIR)/deps

CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
PRECOMPILE = @mkdir -p $(dir $@)
POSTCOMPILE =

ifndef NODEPS

DEPFLAGS = -MT $@ -MMD -MP -MF $(DEP_DIR)/$*.Td
CFLAGS += $(DEPFLAGS)

PRECOMPILE += ;mkdir -p $(dir $(DEP_DIR)/$*)
POSTCOMPILE += @mv -f $(DEP_DIR)/$*.Td $(DEP_DIR)/$*.d && touch $@

endif

include src.mk

OBJS = $(patsubst src/%.c,$(OBJ_DIR)/%.o,$(SRCS))

all: $(NAME)

$(OBJ_DIR)/%.o: src/%.c Makefile
	$(PRECOMPILE)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<
	$(POSTCOMPILE)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

clean:
	rm -rf $(BUILD_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean $(NAME)

include $(wildcard $(DEP_DIR)/*.d)

.PHONY: all clean fclean re
