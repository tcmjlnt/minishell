/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paths.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 11:25:43 by aumartin          #+#    #+#             */
/*   Updated: 2025/05/16 12:04:11 by aumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// SI chemin absolu ou relatif fourni directement
static char	*check_absolute_or_relative_path(char *cmd)
{
	if (ft_strchr(cmd, '/') != NULL)
	{
		if (access(cmd, X_OK) == 0)
			return (gc_strdup(cmd, GC_CMD));
	}
	return (NULL);
}

char	*find_command_path(char *cmd, t_env *env)
{
	char	*path_env;
	char	**paths;
	char	full_path[1024];
	int		i;
	char	*absolute;

	absolute = check_absolute_or_relative_path(cmd);
	if (absolute)
		return (absolute);
	path_env = get_env_value(env, "PATH");
	if (!path_env)
		return (NULL);
	paths = gc_split(path_env, ':', GC_CMD);
	i = 0;
	while (paths[i])
	{
		ft_strlcpy(full_path, paths[i], sizeof(full_path));
		ft_strlcat(full_path, "/", sizeof(full_path));
		ft_strlcat(full_path, cmd, sizeof(full_path));
		if (access(full_path, X_OK) == 0)
			return (gc_strdup(full_path, GC_CMD));
		i++;
	}
	return (NULL);
}

void	print_cmd_path_found(char *cmd, t_env *env)
{
	char	*path;

	path = find_command_path(cmd, env);
	if (path)
		printf("Commande '%s' found: %s\n", cmd, path);
	else
		printf("Commande '%s' not found.\n", cmd);
}
