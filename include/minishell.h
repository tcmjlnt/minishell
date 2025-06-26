/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 11:47:04 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/26 11:52:51 by aumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define MAX_ARGS 255

# define RED "\001\e[1;91m\002"
# define YELLOW "\001\e[1;93m\002"
# define CYAN "\001\e[1;96m\002"
# define GREEN "\001\e[1;92m\002"
# define MAGENTA "\001\e[1;95m\002"
# define RESET "\001\033[0m\002 "
# define STDIN_EOF_WARNING "minishell: warning: "
# define STDIN_EOF_MSG "here-document delimited by end-of-file (wanted `"

/* ==========================    📚 INCLUDES    ========================== */

# include "../lib/libft/libft.h"
# include "../lib/ft_printf/ft_printf.h"
# include "../lib/gnl/get_next_line_bonus.h"
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
// # include <stdbool.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <limits.h>
# include <fcntl.h>
# include <signal.h>

extern int		g_sig;

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
	GC_TKN,
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
	TOKEN_EOF,				// fin de la ligne/commande
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

typedef struct s_redir
{
	t_token_type		type; // a sup uniquement pour tester exec redir
	char				*file;
	char				*delim;
	struct s_redir		*next;
	struct s_redir		*prev;
}	t_redir;

typedef struct s_cmd
{
	char			*cmd;
	char			**args;
	int				fd_in;
	int				fd_out;
	int				pid;
	int				pipe[2];
	t_bool			is_builtin;
	t_redir			*redir;
	struct s_cmd	*next;
	struct s_cmd	*prev;
}	t_cmd;

typedef struct s_shell t_shell;
typedef struct s_cmd t_cmd;

/* typedef de pointeur de fonction. */
typedef int	(*t_builtin_func)(t_shell *, t_cmd *, int);

typedef struct s_builtin
{
	char			*name;
	t_builtin_func	func;
}	t_builtin;

typedef struct s_std_backup
{
	int	orig_stdin;
	int	orig_stdout;
	int	orig_stderr;
}	t_std_backup;

typedef struct s_shell
{
	t_gc			gc;
	t_env			*env;
	char			**paths;
	t_bool			is_cmd; // verifier car je crois que je l'utilise plus finalement
	t_builtin		builtins[8]; // init OK
	int				exit_status;
	t_std_backup	std_backup; // init OK
}	t_shell;

typedef	struct s_token
{
	int				token_type;
	char			*token_value;
	char			*token_raw;
	// char			**args;
	// int			node_num;
	struct s_token	*prev;
	struct s_token	*next;
}	t_token;

typedef	struct s_xpnd
{
	char			*substr;
	// char			*key;
	t_bool			in_single;
	t_bool			in_double;
	t_bool			xpnd_check;
	// char			*xpnd_value;
	char			*str_to_join;
	// t_token			*token_link; // jai peur que ce soit complique a gerer notamment dans les cas ou je ne veux pas expand
	struct s_xpnd	*prev;
	struct s_xpnd	*next;
}	t_xpnd;

/* ===========================    ♻️ PROMPT    =========================== */
void	ft_prompt(t_shell *shell);

/* ======================     🧹 GARBAGE COLLECTOR    ====================== */
void	*gc_mem(t_gc_action op, size_t size, void *ptr, t_gc_type type);
char	*gc_strdup(const char *src, t_gc_type type);
char	*gc_strndup(char *src, size_t n, t_gc_type type);
char	**gc_split(char *str, char sep, t_gc_type type);
char	*gc_strjoin(char const *s1, char const *s2, t_gc_type type);
char	*gc_itoa(int n, t_gc_type type);


/* ==============================    🛠️ UTILS    ================================ */
void	error_exit(const char *message);
char	*gc_strndup_noquotes(char *src, size_t n, t_gc_type type);
size_t	ft_strnlen_noquotes(char *src, size_t n);
char	*ft_strndup(char *src, size_t n);
t_token	*ft_lstnew_token(char *value, int type, int node_num);
void	print_token(t_token	*token_list);
int		get_exit_status(int status);
t_shell	*get_shell(void);
void	init_shell(void);
void	free_and_cleanup_heredocs(t_cmd *cmd_list);
void	error_free_GC_CMD(const char *message);

/* ===========================    🚀 EXECUTION    =========================== */
void	exec_dispatcher(t_cmd *cmd, t_shell *shell);
void	exec_single_cmd(t_cmd *cmd, t_shell *shell);
char	*find_command_path(char *cmd, t_env *env);
void	print_cmd_path_found(char *cmd, t_env *env);
int		apply_redirections(t_cmd *cmd);
void	exec_pipeline(t_cmd *cmd, t_shell *shell);
void	wait_for_children(t_cmd *cmds, t_shell *shell);
t_bool	is_valid_command(t_cmd *cmd, t_shell *shell, int *status, char **path);
int		check_redirections_consistency(t_cmd *cmd);
int		handle_all_heredocs(t_cmd *cmd_list);
void	cleanup_heredocs(t_cmd *cmd_list);

/* ========================    🌱 ENVIRONNEMENT    ======================== */
void	print_envp(char **envp);
void	print_env_line(char *line);
void	print_env(t_env *env);
void	print_env_tab(char **env_tab);
void	env_from_envp(t_shell *shell, char **envp);
char	**env_to_env_tab_for_execve(t_env *env);
char	*get_env_value(t_env *env, const char *key, t_gc_type type);
void	update_env_value(t_env *env, const char *key, const char *new_value);
t_env	*env_new(char *key, char *value, t_bool equal);
void	env_add_back(t_env **lst, t_env *new);



/* ===========================    🔧 BUILTINS    ============================ */
int		ft_pwd(t_shell *shell, t_cmd *cmd, int fd);
int		ft_env(t_shell *shell, t_cmd *cmd, int fd);
int		ft_echo(t_shell *shell, t_cmd *cmd, int fd);
int		ft_export(t_shell *shell, t_cmd *cmd, int fd);
t_env	*check_existing_key(t_shell *shell, char *key);
int		ft_unset(t_shell *shell, t_cmd *cmd, int fd);
int		ft_cd(t_shell *shell, t_cmd *cmd, int fd);
int		ft_exit(t_shell *shell, t_cmd *cmd, int fd);
int		handle_builtin(t_shell *shell, t_cmd *cmd, int fd);
t_bool	is_builtin(t_shell *shell, char *cmd_name);

/* ========================    🦄 PARSING    ======================== */
int		parsing(char *prompt, t_cmd **cmd_list, t_shell *shell);
t_token	*ft_lstlast_token(t_token *token);
void	ft_lstadd_back_token(t_token **token, t_token *new);
t_token	*ft_lstnewtoken(char *prompt, int n, t_token_type token_type);
int		is_operator_token(t_token *token);
int		check_token(t_token **token_list);
int		is_quote(char c);
int		parse_tokens(t_cmd **cmd_list, t_token **tkn_list, t_shell *shell);
t_cmd	*ft_lstnewcmd(void);
void	ft_lstadd_back_cmd(t_cmd **cmd, t_cmd *new);
int		is_inside_squotes(char *token_raw);
int		is_inside_dquotes(char *token_raw);
void	print_args(t_cmd *cmd);
int		is_redir_operator(t_token_type token_type);
int		fill_redir(t_redir **redir, t_token *token);
int		is_inside_dquotes(char *token_raw);
int		is_inside_squotes(char *token_raw);
int		is_blank(int c);


/* ========================    💰 EXPANSION    ======================== */
int		handle_expansion(t_token **tkn_list, t_token **tkn_xpnd_list, t_shell *shell);
t_xpnd	*ft_lstnewxpnd(void);
void	ft_lstadd_back_xpnd(t_xpnd **xpnd, t_xpnd *new);
t_xpnd	*ft_lstlast_xpnd(t_xpnd *xpnd);
t_token	*ft_lstnewtoken_xpnd(void);
void	free_t_xpnd_list(t_xpnd *xpnd_quotes_list);
int		is_valid_keychar(char c);

/* ========================    📡 SIGNALS    ======================== */
void	init_signals(void);
void	signal_handler(int sig);
void	signal_handler_exec(int sig);
void	set_signals_interactive(void);
void	set_signals_exec(void);
void	signal_handler_heredoc(int sig);



/* ========================    🚧 DEBUG    ======================== */
void	debug_env(t_shell *shell);
void	debug_env_tab(t_shell *shell);
void	debug_path_for(char *cmd, t_shell *shell);
void	debug_cmd(t_cmd *cmd);
void	debug_cmd_list(t_cmd *cmds);
void	debug_pipe(int in_fd, int pipe_fd[2]);
void	debug_exec_external(t_cmd *cmd);

#endif
