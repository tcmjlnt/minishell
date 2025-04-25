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
CFLAGS = -Wall -Wextra -Werror -g3
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
	builtins/builtin_unset.c
SRC_ENV = \
	env/env_utils.c \
	env/export_utils.c
SRC_EXECUTOR = \
	executor/exec.c \
	executor/pipes.c \
	executor/redirections.c
SRC_PARSER = \
	parser/expander.c \
	parser/lexer.c \
	parser/parser.c
SRC_SIGNALS = \
	signals/signals.c
SRC_UTILS = \
	utils/error.c \
	utils/memory.c \
	utils/str_utils.c
SRC_FILES = main.c $(SRC_BUILTINS) $(SRC_ENV) $(SRC_EXECUTOR) \
			$(SRC_PARSER) $(SRC_SIGNALS) $(SRC_UTILS)

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
	echo "          _____                    _____                    _____                    _____     _____     _____                    _____             _____                    _____                    _____          "; \
	echo "         /\    \                  /\    \                  /\    \                  /\    \   /\    \   /\    \                  /\    \           /\    \                  /\    \                  /\    \         "; \
	echo "        /::\    \                /::\____\                /::\    \                /::\____\ /::\____\ /::\____\                /::\    \         /::\    \                /::\    \                /::\    \        "; \
	echo "       /::::\    \              /:::/    /               /::::\    \              /:::/    //:::/    //::::|   |               /::::\    \        \:::\    \              /::::\    \              /::::\    \       "; \
	echo "      /::::::\    \            /:::/    /               /::::::\    \            /:::/    //:::/    //:::::|   |              /::::::\    \        \:::\    \            /::::::\    \            /::::::\    \      "; \
	echo "     /:::/\:::\    \          /:::/    /               /:::/\:::\    \          /:::/    //:::/    //::::::|   |             /:::/\:::\    \        \:::\    \          /:::/\:::\    \          /:::/\:::\    \     "; \
	echo "    /:::/__\:::\    \        /:::/____/               /:::/__\:::\    \        /:::/    //:::/    //:::/|::|   |            /:::/__\:::\    \        \:::\    \        /:::/__\:::\    \        /:::/__\:::\    \    "; \
	echo "    \:::\   \:::\    \      /::::\    \              /::::\   \:::\    \      /:::/    //:::/    //:::/ |::|   |           /::::\   \:::\    \       /::::\    \      /::::\   \:::\    \       \:::\   \:::\    \   "; \
	echo "  ___\:::\   \:::\    \    /::::::\    \   _____    /::::::\   \:::\    \    /:::/    //:::/    //:::/  |::|___|______    /::::::\   \:::\    \     /::::::\    \    /::::::\   \:::\    \    ___\:::\   \:::\    \  "; \
	echo " /\   \:::\   \:::\    \  /:::/\:::\    \ /\    \  /:::/\:::\   \:::\    \  /:::/    //:::/    //:::/   |::::::::\    \  /:::/\:::\   \:::\    \   /:::/\:::\    \  /:::/\:::\   \:::\    \  /\   \:::\   \:::\    \ "; \
	echo "/::\   \:::\   \:::\____\/:::/  \:::\    /::\____\/:::/__\:::\   \:::\____\/:::/____//:::/____//:::/    |:::::::::\____\/:::/  \:::\   \:::\____\ /:::/  \:::\____\/:::/__\:::\   \:::\____\/::\   \:::\   \:::\____\\"; \
	echo "\:::\   \:::\   \::/    /\::/    \:::\  /:::/    /\:::\   \:::\   \::/    /\:::\    \\:::\    \\::/    / ~~~~~/:::/    /\::/    \:::\  /:::/    //:::/    \::/    /\:::\   \:::\   \::/    /\:::\   \:::\   \::/    /"; \
	echo " \:::\   \:::\   \/____/  \/____/ \:::\/:::/    /  \:::\   \:::\   \/____/  \:::\    \\:::\    \\/____/      /:::/    /  \/____/ \:::\/:::/    //:::/    / \/____/  \:::\   \:::\   \/____/  \:::\   \:::\   \/____/ "; \
	echo "  \:::\   \:::\    \               \::::::/    /    \:::\   \:::\    \       \:::\    \\:::\    \           /:::/    /            \::::::/    //:::/    /            \:::\   \:::\    \       \:::\   \:::\    \     "; \
	echo "   \:::\   \:::\____\               \::::/    /      \:::\   \:::\____\       \:::\    \\:::\    \         /:::/    /              \::::/    //:::/    /              \:::\   \:::\____\       \:::\   \:::\____\    "; \
	echo "    \:::\  /:::/    /               /:::/    /        \:::\   \::/    /        \:::\    \\:::\    \       /:::/    /               /:::/    / \::/    /                \:::\   \::/    /        \:::\  /:::/    /    "; \
	echo "     \:::\/:::/    /               /:::/    /          \:::\   \/____/          \:::\    \\:::\    \     /:::/    /               /:::/    /   \/____/                  \:::\   \/____/          \:::\/:::/    /     "; \
	echo "      \::::::/    /               /:::/    /            \:::\    \               \:::\    \\:::\    \   /:::/    /               /:::/    /                              \:::\    \               \::::::/    /      "; \
	echo "       \::::/    /               /:::/    /              \:::\____\               \:::\____\\:::\____\ /:::/    /               /:::/    /                                \:::\____\               \::::/    /       "; \
	echo "        \::/    /                \::/    /                \::/    /                \::/    / \::/    / \::/    /                \::/    /                                  \::/    /                \::/    /        "; \
	echo "         \/____/                  \/____/                  \/____/                  \/____/   \/____/   \/____/                  \/____/                                    \/____/                  \/____/         "
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
	@$(PRINT_LOGO)

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
