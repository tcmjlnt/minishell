/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 14:17:01 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/21 18:19:22 by aumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Implémenter la commande builtin `cd`
// Mettre à jour PWD et OLDPWD dans l'environnement
// Gérer les erreurs et messages appropriés

#include "../../include/minishell.h"

/* gere les cas speciaux comme '~', '-', ou autres erreurs */
static int	handle_special_cases(t_cmd *cmd)
{
	if (!cmd->args[0])
	{
		ft_putstr_fd("minishell: cd: missing path\n", STDERR_FILENO);
		ft_putstr_fd("Usage: cd <path> relative or absolute\n", STDERR_FILENO);
		return (1);
	}
	if (ft_strncmp(cmd->args[0], "~", 1) == 0)
	{
		ft_putstr_fd("minishell: cd: '~' not supported \n", STDERR_FILENO);
		ft_putstr_fd("Usage: cd: $HOME or cd <path>\n", STDERR_FILENO);
		return (1);
	}
	if (ft_strncmp(cmd->args[0], "-", 1) == 0)
	{
		ft_putstr_fd("minishell: cd: '-' not supported \n", STDERR_FILENO);
		ft_putstr_fd("Usage: cd <path> relative or absolute\n", STDERR_FILENO);
		return (1);
	}
	return (0);
}

/* handle_special_cases() traite cd seul, cd ~, cd -.
ft_cd() applique getcwd
chdir, met à jour OLDPWD et PWD en utilisant gc_strdup
free() pour getcwd() qui est un malloc. */

int	ft_cd(t_shell *shell, t_cmd *cmd, int fd)
{
	char	*old_pwd;
	char	*new_pwd;

	(void)fd;
	if (handle_special_cases(cmd))
		return (shell->exit_status = 1, 1);
	old_pwd = getcwd(NULL, 0);
	if (!old_pwd)
		return (perror("minishell: cd: getcwd"), 1);
	if (chdir(cmd->args[1]) != 0)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		perror(cmd->args[0]);
		free(old_pwd);
		return (1);
	}
	new_pwd = getcwd(NULL, 0);
	if (!new_pwd)
		return (perror("minishell: cd: getcwd"), free(old_pwd), 1);
	update_env_value(shell->env, "OLDPWD", gc_strdup(old_pwd, GC_ENV));
	update_env_value(shell->env, "PWD", gc_strdup(new_pwd, GC_ENV));
	free(old_pwd);
	free(new_pwd);
	shell->exit_status = 0;
	return (0);
}
