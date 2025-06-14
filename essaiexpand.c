/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   essaiexpand.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 18:27:22 by tjacquel          #+#    #+#             */
/*   Updated: 2025/06/14 20:20:06 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>



char	*ft_strndup(char *src, size_t n)
{
	char	*dest;
	size_t		i;

	dest = malloc(sizeof(char) * (n + 1));
	if (!dest)
		return(NULL);
	i = 0;
	while (i < n && src[i])
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

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

char	**char_keys(char *tkn_raw, size_t count_expand)
{
	size_t	i;
	size_t	start;
	size_t	count_keys;

	bool	in_single;
	bool	in_double;
	char	**key = malloc(sizeof(char *) * (count_expand + 1));
	if (!key)
		return (NULL);
	i = 0;
	start = 0;
	count_keys = 0;

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
		{
			i++;
			start = i;
			while (tkn_raw[i] && is_valid_keychar(tkn_raw[i]))
				i++;
			key[count_keys] = ft_strndup(tkn_raw + start, i - start);
			if (!key[count_keys])
			{
				while (count_keys > 0)
					free(key[--count_keys]);
				free(key);
				return (NULL);
			}
			count_keys++;
			if (!tkn_raw[i])
				break ;
		}
		else
			i++;
	}
	key[count_expand] = NULL;
	return (key);
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
		else
		{
			// while ()
			//count_expand++;
		}
		i++;
	}
	return (count_expand);
}


size_t	count_segment(char *tkn_raw)
{
	size_t	i;
	size_t	count_segment;
	size_t	current_literal_length;
	bool	in_single;
	bool	in_double;

	i = 0;
	count_segment = 0;
	current_literal_length = 0;
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
		{
			if (current_literal_length > 0)
				count_segment++;
			current_literal_length = 0;
			count_segment++;
			i++;
			while (tkn_raw[i] && is_valid_keychar(tkn_raw[i]))
				i++;
			continue ;
		}
		else
		{
			current_literal_length++;
			i++;
		}
	}
	if (current_literal_length > 0)
		count_segment++;
	return (count_segment);
}

int	main(void)
{
	// char	*arg1="$USER\"$USER\"\'$USER\'$US\"ER\"";
	// char	*arg2 = "echo \"$Abba\'$Bebe\"\'$Coucou\"$Didier\'$Elephant\'\"$Fanny\'\"\'$Gold\'$Hi\"";
	char	*arg3="\'$USER $USER\'$USER";

	size_t	count = count_expand(arg1);
	char **key;


	key = char_keys(arg1, count);
	if (key)
	{
		for (size_t i = 0 ; i < count ; i++)
		{
			printf("arg: `%s`\nchar_keys: `%s`\n%td\n", arg1, key[i], count);
			free(key[i]);
		}
		free(key);
	}
	return (0);
}