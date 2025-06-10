/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tests.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 16:33:25 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/10 09:04:43 by aumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TESTS_H
# define TESTS_H

# include "../include/minishell.h"

void	debug_env(t_shell *shell);
void	debug_env_tab(t_shell *shell);
void	debug_path_for(char *cmd, t_shell *shell);

void	print_envp(char **envp);
void	print_env_line(char *line);
void	print_env(t_env *env);
void	print_env_tab(char **env_tab);
void	print_cmd_path_found(char *cmd, t_env *env);

void	test_export_var1(t_shell *shell);
void	test_export_var2(t_shell *shell);
void	test_builtin_pwd(t_shell *shell, t_cmd *cmd);
void	test_builtin_env(t_shell *shell, t_cmd *cmd);
void	test_builtin_echo(t_shell *shell, t_cmd *cmd);
void	test_handle_builtin(t_shell *shell, t_cmd *cmd);
void	run_all_tests(t_shell *shell);
void	run_redirections_tests(t_shell *shell);

#endif
