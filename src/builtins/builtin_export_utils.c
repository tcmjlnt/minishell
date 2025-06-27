/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 18:45:30 by tjacquel          #+#    #+#             */
/*   Updated: 2025/06/27 18:45:48 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	print_env_export(t_env *env)
{
	while (env)
	{
		printf("export %s=", env->key);
		if (env->value)
			printf("\"%s\"", env->value);
		printf("\n");
		env = env->next;
	}
}
