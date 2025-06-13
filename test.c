/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 18:27:22 by tjacquel          #+#    #+#             */
/*   Updated: 2025/06/13 19:21:49 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdbool.h>


int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

int	ft_isalpha(int c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return (1);
	return (0);
}

static int	ft_isalnum(int c)
{
	if (ft_isalpha(c) || ft_isdigit(c))
		return (1);
	else
		return (0);
}

static int is_valid_keychar(char c)
{
	if (ft_isalnum(c) || c == '_')
		return (1);
	else
		return (0);
}



size_t	count_expand(char *tkn_raw)
{
	size_t	i;
	size_t	count_expand;
	bool	in_single;
	bool	in_double;

	i = 0;
	count_expand = 0;
	in_double = false;
	in_single = false;
	while(tkn_raw[i])
	{
		if (tkn_raw[i] == '\'' && !in_double)
			in_single = !in_single; // toggle in_single state
		else if (tkn_raw[i] == '\"' && !in_single)
			in_double = !in_double; // toggle in_double state
		if (tkn_raw[i] == '$' && tkn_raw[i + 1] && is_valid_keychar(tkn_raw[i + 1])
				&& !in_single)
			count_expand++;
		i++;
	}
	return (count_expand);
}

int	main(void)
{
	char	*arg1 = "echo \"$A\'$B\"\'$C\"$D\'$E\'\"$F\'\"\'$G\'$H\"";
	size_t	count = count_expand(arg1);
	printf("arg: `%s`\n%td\n", arg1, count);
	return (0);
}