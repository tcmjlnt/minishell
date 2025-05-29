/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 19:28:44 by tjacquel          #+#    #+#             */
/*   Updated: 2025/05/29 13:42:59 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

char	which_quote(char c)
{
	if (c == '\'')
		return ('\'');
	else if (c == '\"')
		return ('\"');
	return (0);
}
int	main(int argc, char **argv)
{
	int i = 0;
	int start = 0;
	if (argc == 2)
	while (argv[1][i])
	{
		if (which_quote(argv[1][i]))
		{
			char quote_type = argv[1][i];
			i++;
			start = i;
			printf("%c", argv[1][start--]);
			while (argv[1][i] && argv[1][i] !=  quote_type)
			{
				printf("%c", argv[1][i]);
				i++;
			}
			if (argv[1][i] == quote_type)
				i++;
		}
		else
		{
			printf("%c", argv[1][i]);
			i++;
		}
	}
	return (0);

}
