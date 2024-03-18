/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 18:14:41 by caguillo          #+#    #+#             */
/*   Updated: 2024/03/18 00:08:23 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <fcntl.h>
# include <stdlib.h>
# include <unistd.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1
# endif

char	*get_next_line(int fd);
char	*get_next_stash(int fd, char *keep);
char	*initialize_stash(char *keep);
ssize_t	is_nl(char *str);

char	*gnl_strjoin(char *stash, char *buff);
char	*ft_substr(char *s, size_t start, size_t len);
size_t	ft_strlen(char *str);
void	*ft_calloc(size_t nmemb, size_t size);

#endif
