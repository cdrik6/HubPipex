/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 14:36:17 by caguillo          #+#    #+#             */
/*   Updated: 2024/03/03 02:32:27 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*get_next_line(int fd)
{
	static char	keep[BUFFER_SIZE];
	char		*stash;
	char		*line;
	char		*stash_next;
	size_t		i;

	if (fd < 0 || BUFFER_SIZE < 1)
		return (NULL);
	stash = get_next_stash(fd, keep);
	if (stash == NULL || stash[0] == '\0')
		return (keep[0] = '\0', free(stash), NULL);
	line = ft_substr(stash, 0, is_nl(stash) + 1);
	stash_next = ft_substr(stash, is_nl(stash) + 1, ft_strlen(stash));
	free(stash);
	if (line == NULL || stash_next == NULL)
		return (keep[0] = '\0', NULL);
	if (line[0] == '\0')
		return (keep[0] = '\0', free(line), stash_next);
	i = 0;
	while (stash_next[i])
	{
		keep[i] = stash_next[i];
		i++;
	}
	return (keep[i] = '\0', free(stash_next), line);
}

char	*get_next_stash(int fd, char *keep)
{
	char	*buff;
	char	*stash;
	int		eof;
	ssize_t	nbyte;

	eof = 0;
	stash = initialize_stash(keep);
	buff = ft_calloc(BUFFER_SIZE + 1, sizeof(char));
	if (!buff)
		return (free(stash), NULL);
	while (is_nl(stash) == -1 && eof == 0)
	{
		nbyte = read(fd, buff, BUFFER_SIZE);
		if (nbyte < 0)
			return (free(stash), free(buff), NULL);
		if (nbyte < BUFFER_SIZE)
		{
			eof = 1;
			buff[nbyte] = '\0';
		}
		stash = gnl_strjoin(stash, buff);
	}
	return (free(buff), stash);
}

ssize_t	is_nl(char *str)
{
	size_t	i;

	if (str == NULL)
		return (-1);
	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '\n')
			return (i);
		i++;
	}
	return (-1);
}

char	*initialize_stash(char *keep)
{
	char	*stash;
	int		i;

	stash = malloc(sizeof(char) * (ft_strlen(keep) + 1));
	if (!stash)
		return (NULL);
	i = 0;
	while (keep[i])
	{
		stash[i] = keep[i];
		i++;
	}
	stash[i] = '\0';
	return (stash);
}
