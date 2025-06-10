NAME = minishell_tests
CC = cc
CFLAGS = -Wall -Wextra -Werror -g

SRC_TESTS = test.c debug.c main.c test_builtins.c test_exec_redir.c
SRC_MAIN = \
	../src/env/env_build.c \
	../src/env/env_tab_build.c \
	../src/env/print.c \
	../src/exec/exec_with_redir.c \
	../src/exec/paths.c \
	../src/utils/gc_utils.c \
	../src/utils/init_struct.c \
	../src/utils/garbage_collector.c \
	../src/utils/error_management.c \
	../src/builtins/handle_builtins.c \
	../src/builtins/builtin_echo.c \
	../src/builtins/builtin_env.c \
	../src/builtins/builtin_pwd.c \
	../src/exec/redir_exec.c \
	../src/exec/redir_handle.c \
	../src/exec/redir_open.c \

LIB_DIR = ../lib
LIB = $(LIB_DIR)/lib.a
MAKE_LIB = make -sC $(LIB_DIR)

INCLUDE = -I../include -I$(LIB_DIR)/libft -I$(LIB_DIR)/ft_printf -I$(LIB_DIR)/gnl

all: $(LIB)
	$(CC) $(CFLAGS) $(SRC_TESTS) $(SRC_MAIN) $(LIB) -o $(NAME) $(INCLUDE) -lreadline

run: all
	./$(NAME)

$(LIB):
	@$(MAKE_LIB)

clean:
	rm -f $(NAME)

re: clean all
