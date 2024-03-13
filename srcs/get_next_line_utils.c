/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 14:36:17 by caguillo          #+#    #+#             */
/*   Updated: 2024/03/13 00:59:23 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*gnl_strjoin(char *stash, char *buff)
{
	char	*join;
	size_t	i;
	size_t	j;

	if (stash == NULL || buff == NULL)
		return (free(stash), NULL);
	join = malloc(sizeof(char) * (ft_strlen(stash) + ft_strlen(buff) + 1));
	if (!join)
		return (free(stash), NULL);
	i = 0;
	j = 0;
	while (stash[i])
	{
		join[i] = stash[i];
		i++;
	}
	while (buff[j])
	{
		join[i + j] = buff[j];
		j++;
	}
	join[i + j] = '\0';
	free(stash);
	return (join);
}

size_t	ft_strlen(char *str)
{
	size_t	i;

	if (!str)
		return (0);
	i = 0;
	while (str[i])
		i++;
	return (i);
}

char	*ft_substr(char *s, size_t start, size_t len)
{
	char	*sub;
	size_t	i;
	size_t	size;

	if (!s)
		return (NULL);
	if (start >= ft_strlen(s))
		size = 0;
	else if (start + len > ft_strlen(s))
		size = ft_strlen(s) - start;
	else
		size = len;
	i = start;
	sub = malloc(sizeof(char) * (size + 1));
	if (!sub)
		return (NULL);
	while (i < size + start)
	{
		sub[i - start] = s[i];
		i++;
	}
	sub[i - start] = '\0';
	return (sub);
}

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*str;
	size_t	i;

	i = 0;
	if (size != 0 && nmemb > ((size_t)(-1)) / size)
		return (NULL);
	str = malloc(nmemb * size);
	if (!str)
		return (NULL);
	while (i < nmemb * size)
	{
		((unsigned char *)str)[i] = 0;
		i++;
	}
	return (str);
}
