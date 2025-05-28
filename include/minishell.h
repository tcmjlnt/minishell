/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 11:47:04 by aumartin          #+#    #+#             */
/*   Updated: 2025/05/28 11:38:17 by aumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* ==========================    📚 INCLUDES    ========================== */

# include "../lib/libft/libft.h"
# include "../lib/ft_printf/ft_printf.h"
# include "../lib/gnl/get_next_line_bonus.h"
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <errno.h>
# include <sys/types.h>
# include <sys/wait.h>

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
	char		**paths;
	int			cmd_count;
	t_bool		is_cmd;
	int			exit_status;
}	t_shell;

typedef struct s_cmd
{
	char			*cmd;
	char			**args;
	int				input;
	int				output;
	int				pid;
	t_bool			is_builtin;
	struct s_cmd	*next;
	struct s_cmd	*prev;
}	t_cmd;

/* ===========================    ♻️ PROMPT    =========================== */
void	ft_prompt(void);

/* ======================     🧹 GARBAGE COLLECTOR    ====================== */
void	*gc_mem(t_gc_action op, size_t size, void *ptr, t_gc_type type);
char	*gc_strdup(const char *src, t_gc_type type);
char	**gc_split(char *str, char sep, t_gc_type type);

/* ===========================    🛠️ UTILS    ============================== */
void	error_exit(const char *message);
t_shell	*get_shell(void);
void	init_shell(void);

/* ===========================    🚀 EXECUTION    =========================== */
char	*find_command_path(char *cmd, t_env *env);
void	print_cmd_path_found(char *cmd, t_env *env);
void	exec_cmd(t_cmd *cmd, t_env *env);

/* ========================    🌱 ENVIRONNEMENT    ======================== */
void	print_envp(char **envp);
void	print_env_line(char *line);
void	print_env(t_env *env);
void	print_env_tab(char **env_tab);
void	env_from_envp(t_shell *shell, char **envp);
char	*get_env_value(t_env *env, const char *key);
char	**env_to_env_tab_for_execve(t_env *env);

/* ===========================    🔧 BUILTINS    ============================ */

#endif
