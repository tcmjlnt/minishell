/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tests.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 16:33:25 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/05 15:42:09 by aumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TESTS_H
# define TESTS_H

# include "../include/minishell.h"

void	debug_env(t_shell *shell);
void	debug_env_tab(t_shell *shell);
void	debug_path_for(char *cmd, t_shell *shell);

void	test_export_var1(t_shell *shell);
void	test_export_var2(t_shell *shell);
void	test_builtin_pwd(t_shell *shell, t_cmd *cmd);
void	test_builtin_env(t_shell *shell, t_cmd *cmd);
void	test_builtin_echo(t_shell *shell, t_cmd *cmd);
void	test_handle_builtin(t_shell *shell, t_cmd *cmd);
void	test_exec_cmd(t_shell *shell, t_cmd *cmd);
void	test_pipe_ls_wc(t_shell *shell);
void	test_pipe_pwd_echo_ls(t_shell *shell);
void	test_pipe_echo_cat_redir(t_shell *shell);
void	test_pipe_unknown_cmd(t_shell *shell);
void	test_pipe_single_builtin(t_shell *shell);
void	run_exec_pipes_tests(t_shell *shell);

#endif
