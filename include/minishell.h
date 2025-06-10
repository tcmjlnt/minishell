/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 11:47:04 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/10 18:09:32 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define MAX_ARGS 255

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
# include <errno.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <limits.h>
# include <fcntl.h>

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
	TOKEN_EOF,				// fin de la ligne/commande
	ble,
	bla,
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
	struct s_redir		*next;
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

typedef struct s_exec
{
	int		in_fd;
	int		pipe_fd[2];
	pid_t	pid;
}	t_exec;

typedef struct s_shell
{
	t_gc		gc;
	t_env		*env;
	char		**paths;
	t_bool		is_cmd;
	t_builtin	builtins[8];
	int			exit_status;
}	t_shell;

typedef	struct s_token
{
	int				token_type;
	char			*token_value;
	char			*token_raw;
	// char			**args;
	// int				node_num;
	struct s_token	*prev;
	struct s_token	*next;
}	t_token;

/* ===========================    ♻️ PROMPT    =========================== */
void	ft_prompt(t_shell *shell);

/* ======================     🧹 GARBAGE COLLECTOR    ====================== */
void	*gc_mem(t_gc_action op, size_t size, void *ptr, t_gc_type type);
char	*gc_strdup(const char *src, t_gc_type type);
char	**gc_split(char *str, char sep, t_gc_type type);

/* ==============================    🛠️ UTILS    ================================ */
void	error_exit(const char *message);
char	*ft_strndup_noquotes(char *src, size_t n);
size_t	ft_strnlen_noquotes(char *src, size_t n);
char	*ft_strndup(char *src, size_t n);
t_token	*ft_lstnew_token(char *value, int type, int node_num);
void	print_token(t_token	*token);

/* ===========================    🔧 BUILTINS    ============================ */
int		get_exit_status(int status);
t_shell	*get_shell(void);
void	init_shell(void);

/* ===========================    🚀 EXECUTION    =========================== */
char	*find_command_path(char *cmd, t_env *env);
void	print_cmd_path_found(char *cmd, t_env *env);
void	exec_external_cmd(t_cmd *cmd, t_shell *shell);
void	parent_close_fds(t_exec *exec);
void	pipe_reset(int pipe_fd[2]);
void	pipe_create(int pipe_fd[2]);
void	wait_pipeline(t_cmd *cmds);
void	exec_dispatcher(t_cmd *cmds, t_shell *shell);
void	exec_single_cmd(t_cmd *cmd, t_shell *shell);
void	exec_first_child(t_cmd *cmd, t_shell *shell);
void	exec_middle_child(t_cmd *cmd, t_shell *shell);
void	exec_last_child(t_cmd *cmd, t_shell *shell);
void	exec_pipeline(t_cmd *cmd, t_shell *shell);
void	apply_dup_redirections(t_cmd *cmd);
void	prepare_child(t_cmd *cmd, t_shell *shell);
void	close_redirections(t_cmd *cmd);
int		open_file(t_token_type type, char *file);
int		handle_single_redirection(t_cmd *cmd, t_redir *redir);
int		apply_redirections(t_cmd *cmd);

/* ========================    🌱 ENVIRONNEMENT    ======================== */
void	print_envp(char **envp);
void	print_env_line(char *line);
void	print_env(t_env *env);
void	print_env_tab(char **env_tab);
void	env_from_envp(t_shell *shell, char **envp);
char	*get_env_value(t_env *env, const char *key);
char	**env_to_env_tab_for_execve(t_env *env);

/* ===========================    🔧 BUILTINS    ============================ */
int		ft_pwd(t_shell *shell, t_cmd *cmd, int fd);
int		ft_env(t_shell *shell, t_cmd *cmd, int fd);
int		ft_echo(t_shell *shell, t_cmd *cmd, int fd);
int		handle_builtin(t_shell *shell, t_cmd *cmd, int fd);
t_bool	is_builtin(t_shell *shell, char *cmd_name);

t_cmd	*create_cmd(char *cmd_name, char **args, int fd_in, int fd_out, t_shell *shell);
void	add_cmd(t_cmd **head, t_cmd *new_cmd);

/* ========================    🦄 PARSING    ======================== */
int		parsing(char *prompt, t_cmd **cmd_list, t_shell *shell);
t_token	*ft_lstlast_token(t_token *token);
void	ft_lstadd_back_token(t_token **token, t_token *new);
t_token	*ft_lstnewtoken(char *prompt, int n, t_token_type token_type);
int		is_operator_token(t_token *token);
int		check_token(t_token *token);
int		is_quote(char c);
int		parse_tokens(t_cmd **cmd_list, t_token **tkn_list);
t_cmd	*ft_lstnewcmd(void);
void	ft_lstadd_back_cmd(t_cmd **cmd, t_cmd *new);
int		handle_expansion(t_token **tkn_list, t_shell *shell);
int		is_inside_squotes(char *token_raw);
int		is_inside_dquotes(char *token_raw);
void	print_args(t_cmd *cmd);


#endif
