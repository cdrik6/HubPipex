/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 22:22:41 by caguillo          #+#    #+#             */
/*   Updated: 2024/03/02 22:46:06 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	ft_putstr_fd(int fd, char *str)
{
	if (!str)
		return ;
	write(fd, str, ft_strlen(str));
}

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
		i++;
	return (i);
}

// check if s2 is in s1 from start
int	check_instr(const char *s1, const char *s2)
{
	int	i;
	int	n;

	i = 0;
	n = ft_strlen(s2);
	if (!s1 || !s2)
		return (0);
	while (s1[i] && s2[i] && i < n)
	{
		if (s1[i] == s2[i])
			i++;
		else
			return (0);
	}
	if (i == n)
		return (1);
	return (0);
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
