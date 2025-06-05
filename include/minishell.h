/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 11:47:04 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/05 18:52:16 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define MAX_ARGS 255

/* ==========================    📚 INCLUDES    ========================== */

# include "../lib/libft/libft.h"
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
// # include <stdbool.h>
# include <readline/readline.h>
# include <readline/history.h>

/* ===========================    🔷 ENUMS    =========================== */

typedef enum e_gc_action
{
	GC_ALLOC,
	GC_FREE_ONE,
	GC_FREE_ALL
}	t_gc_action;

typedef enum e_gc_type
{
	GC_NONE,
	GC_ENV,
	GC_CMD,
	GC_TMP
}	t_gc_type;

typedef enum e_bool
{
	false,
	true
}	t_bool;

typedef enum e_token_type
{
	TOKEN_WORD,				// mot (commande ou argument)
	TOKEN_BLANK,			// SPACE OR TAB
	TOKEN_D_QUOTES,			// "
	TOKEN_S_QUOTES,			// '
	TOKEN_PIPE,				// |
	TOKEN_REDIRECT_IN,		// <
	TOKEN_REDIRECT_OUT,		// >
	TOKEN_REDIRECT_APPEND,	// >>
	TOKEN_REDIRECT_HEREDOC,	// <<
	TOKEN_EOF				// fin de la ligne/commande
}	t_token_type;

typedef enum	e_parsing_type
{
	OPERATOR,
	LITERAL_STRING,
	CMD,

}	t_parsing_type;

/* ==========================    📦 STRUCTURES    ========================== */

typedef struct s_gc_node
{
	void				*ptr;
	t_gc_type			type;
	struct s_gc_node	*next;
}	t_gc_node;

typedef struct s_gc
{
	t_gc_node	*head;
}	t_gc;

typedef struct s_env
{
	char			*key;
	t_bool			equal;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_shell
{
	t_gc		gc;
	t_env		*env;
}	t_shell;

typedef struct s_redir
{
	//t_token			type;       // IN, OUT, APPEND, HEREDOC
	char			*file_name; // cible de la redir
	struct s_redir	*next;
}	t_redir;

typedef struct s_cmd
{
	char			*cmd;       // nom de la commande ("ls")
	char			**args;     // tableau args {"ls", "-l", NULL}
	t_redir			*redir;     // liste chainee des redir
	int				pipe[2];    // pipe[0] = read, pipe[1] = write
	pid_t			pid;        // PID si forke
	t_bool			is_builtin; // pour traitement a part
	int				exit_code;  // code retour
	struct s_cmd	*next;      // commande suivante (pipeline)
	struct s_cmd	*prev;
}	t_cmd;

typedef	struct s_token
{
	int				token_type;
	char			*token_value;
	// char			**args;
	int				node_num;
	struct s_token	*prev;
	struct s_token	*next;
}	t_token;


/* ===========================    ♻️ PROMPT    =========================== */
void	ft_prompt(t_shell *shell);

/* ======================     🧹 GARBAGE COLLECTOR    ====================== */
void	*gc_mem(t_gc_action op, size_t size, void *ptr, t_gc_type type);
char	*gc_strdup(const char *src, t_gc_type type);
char	**gc_split(char *str, char sep, t_gc_type type);

/* ===========================    🥾 INIT    ============================== */
t_shell	*get_shell(void);
void	init_shell(void);
void	init_token(t_token *token);

/* ===========================    🛠️ UTILS    ============================== */
void	error_exit(const char *message);
char	*ft_strndup_noquotes(char *src, size_t n);
size_t	ft_strlen_noquotes(char *src, size_t n);
char	*ft_strndup(char *src, size_t n);
t_token	*ft_lstnew_token(char *value, int type, int node_num);
void	print_token(t_token	*token);


/* ===========================    🚀 EXECUTION    =========================== */
char	*find_command_path(char *cmd, t_env *env);
void	print_cmd_path_found(char *cmd, t_env *env);

/* ========================    🌱 ENVIRONNEMENT    ======================== */
void	print_envp(char **envp);
void	print_env_line(char *line);
void	print_env(t_env *env);
void	env_from_envp(t_shell *shell, char **envp);
char	*get_env_value(t_env *env, const char *key);

/* ========================    🦄 PARSING    ======================== */
int		parsing(char *prompt, t_shell *shell);
t_token	*ft_lstlast_token(t_token *token);
void	ft_lstadd_back_token(t_token **token, t_token *new);
t_token	*ft_lstnewtoken(char *prompt, int n, t_token_type token_type);
int		is_operator_token(t_token *token);
int		check_token(t_token *token);
int		is_quote(char c);






/* ===========================    🔧 BUILTINS    ============================ */

#endif
