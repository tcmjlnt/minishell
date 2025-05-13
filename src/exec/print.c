/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 09:46:11 by aumartin          #+#    #+#             */
/*   Updated: 2025/05/13 10:41:46 by aumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	print_envp(char **envp)
{
	int	i;

	i = 0;
	while (envp[i] != NULL)
		printf("%s\n", envp[i++]);
}

void	print_env_line(char *line)
{
	char *equal;

	equal = ft_strchr(line, '=');
	if (!equal)
		return ;
	write(1, "KEY: ", 5);
	write(1, line, equal - line);
	write(1, "\nVALUE: ", 8);
	write(1, equal + 1, ft_strlen(equal + 1));
	write(1, "\n\n", 2);
}

