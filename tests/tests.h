/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tests.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 16:33:25 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/04 09:44:30 by aumartin         ###   ########.fr       */
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
void	test_exec_echo(void);
void	test_builtin_pwd(t_shell *shell);
void	test_builtin_env(t_shell *shell);
void	test_builtin_echo(t_shell *shell);
void	test_handle_builtin(t_shell *shell);

#endif
