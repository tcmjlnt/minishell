V = false

ifeq ($(V),true)
	ECHO =
else
	ECHO = @
endif

# Nom du programme
NAME = minishell

# Compilation
CC = cc
CFLAGS = -Wall -Wextra -g3 #-fsanitize=address
OS = $(shell uname | tr '[:upper:]' '[:lower:]')

MAKE = make -sC
MKDIR = mkdir -p
RM = rm -rf

# Lib perso
LIB_DIR = lib
LIB = $(LIB_DIR)/lib.a
LINKER = $(LIB)

# Info système
$(info 🖥️ OS détecté : $(OS))

# Bibliothèques système
ifeq ($(OS), linux)
	LINKER += -lreadline -L/usr/lib
	INCLUDE_FLAG += -I/usr/include
else ifeq ($(OS), darwin)
	LINKER += -lreadline -L/opt/homebrew/opt/readline/lib
	INCLUDE_FLAG += -I/opt/homebrew/opt/readline/include
endif

# Includes
INCLUDE_DIR = include
LIB_SUBDIR = $(wildcard $(LIB_DIR)/*)

INCLUDE_FLAG = -I$(INCLUDE_DIR) \
			   $(foreach dir, $(LIB_SUBDIR), -I$(dir))

INCLUDE = $(wildcard $(INCLUDE_DIR)/*.h) \
		  $(foreach dir, $(LIB_SUBDIR), $(wildcard $(dir)/*.h))

# Dossiers sources
SRC_DIR = src/

# Fichiers sources (relatifs à SRC_DIR)
SRC_BUILTINS = \
	builtins/builtin_cd.c \
	builtins/builtin_echo.c \
	builtins/builtin_env.c \
	builtins/builtin_exit.c \
	builtins/builtin_export.c \
	builtins/builtin_pwd.c \
	builtins/builtin_unset.c \
	builtins/handle_builtins.c

SRC_UTILS = \
	utils/error_management.c \
	utils/garbage_collector.c \
	utils/gc_utils.c \
	utils/init_struct.c \
	utils/functions_utils.c \

SRC_PROMPT = \
	prompt/prompt.c \

SRC_ENV = \
	env/print.c \
	env/env_build.c \
	env/env_tab_build.c \
	env/env_access.c \

SRC_EXEC = \
	exec2/paths.c \
	exec2/checks_full_cmd.c \
	exec2/exec_dispatcher.c \
	exec2/exec_single_cmd.c \
	exec2/heredocs.c \
	exec2/pipes.c \
	exec2/redirections.c \

SRC_PARSING = \
	parsing/parsing.c \
	parsing/parsing_utils.c \
	parsing/token_utils.c \
	parsing/parsing_cmd.c \
	parsing/parsing_redir.c \
	parsing/expansion.c \
	parsing/expansion_utils.c

SRC_DEBUG = debug/debug.c \

SRC_FILES = main.c $(SRC_BUILTINS) \
			$(SRC_UTILS) $(SRC_PROMPT) \
			$(SRC_ENV) \
			$(SRC_EXEC) \
			$(SRC_PARSING) \
			$(SRC_DEBUG) \


# Chemins complets des sources et objets
SRCS = $(addprefix $(SRC_DIR), $(SRC_FILES))
OBJS_DIR = objs/
OBJS = $(addprefix $(OBJS_DIR), $(SRC_FILES:.c=.o))

# Couleurs ANSI
GREEN = \033[32m
RED = \033[31m
RESET = \033[0m
YELLOW = \033[33m
CLEAR_LINE = \033[2K\r

# Logo cool
PRINT_LOGO = \
	echo "   _____ __ __    ___  _      _      ___ ___   ____  ______    ___  _____"; \
	echo "  / ___/|  |  |  /  _]| |    | |    |   |   | /    ||      |  /  _]/ ___/"; \
	echo " (   \_ |  |  | /  [_ | |    | |    | _   _ ||  o  ||      | /  [_(   \_ "; \
	echo "  \__  ||  _  ||    _]| |___ | |___ |  \_/  ||     ||_|  |_||    _]\__  |"; \
	echo "  /  \ ||  |  ||   [_ |     ||     ||   |   ||  _  |  |  |  |   [_ /  \ |"; \
	echo "  \    ||  |  ||     ||     ||     ||   |   ||  |  |  |  |  |     |\    |"; \
	echo "   \___||__|__||_____||_____||_____||___|___||__|__|  |__|  |_____| \___|"; \
	echo "                                                                         "; \
	echo "                                           "; \
	echo "              SHELLMATES                  "; \
	echo ""; \
	echo "  ┌─────────────────────────────┐"; \
	echo "  │  Partnered to code & debug  │"; \
	echo "  │   through bugs and segfault │"; \
	echo "  │    for better or for bash   │"; \
	echo "  └─────────────────────────────┘"; \
	echo "                                           "; \
	echo "     tjacquel 🤝 aumartin"

# Compilation principale
all: $(LIB) $(NAME)
# @$(PRINT_LOGO)

# Compilation lib
$(LIB):
	@echo "$(YELLOW)📦 Compilation de la lib...$(RESET)\r"
	@$(MAKE) $(LIB_DIR) > /dev/null 2>&1 \
	&& echo -e "$(CLEAR_LINE)✅ Compilation lib réussie (✔)" \
	|| { echo -e "$(CLEAR_LINE)❌ Erreur : Compilation de la lib échouée (✘)"; exit 1; }

# Règle pour objets
$(OBJS_DIR):
	@$(MKDIR) $(OBJS_DIR)

$(OBJS_DIR)%.o: $(SRC_DIR)%.c $(INCLUDE)
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INCLUDE_FLAG) -c $< -o $@

$(OBJS_DIR)main.o: main.c $(INCLUDE) | $(OBJS_DIR)
	@$(CC) $(CFLAGS) $(INCLUDE_FLAG) -c $< -o $(OBJS_DIR)main.o

# Compilation du binaire
$(NAME): $(OBJS) $(LIB)
	@echo "🚀 Compilation de $(NAME)..."
	@$(CC) $(CFLAGS) $(OBJS) $(LINKER) -o $(NAME) \
	&& echo "✅ $(NAME) a été créé avec succès (✔)" \
	|| { echo "❌ Erreur : Compilation de $(NAME) échouée (✘)"; exit 1; }

# Nettoyage
clean:
	@echo "$(YELLOW)🧹 Nettoyage clean en cours...$(RESET)\r"
	@$(RM) $(OBJS_DIR)
	@echo -e "$(CLEAR_LINE)✅ Nettoyage clean réussi (✔)"

fclean: clean
	@echo "$(YELLOW)🧼 Nettoyage complet fclean en cours...$(RESET)\r"
	@$(RM) $(NAME)
	@$(MAKE) $(LIB_DIR) fclean
	@echo -e "$(CLEAR_LINE)✅ Nettoyage complet fclean réussi (✔)"

re: fclean all

.PHONY: all clean fclean re
