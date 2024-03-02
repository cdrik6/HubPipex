/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 22:30:23 by caguillo          #+#    #+#             */
/*   Updated: 2024/03/02 22:53:42 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

//
# include <errno.h>
//
# include <fcntl.h>
// perror
# include <stdio.h>
// exit code
# include <stdlib.h>
// waitpid
# include <sys/wait.h>
// write, execve
# include <unistd.h>

# define STDIN 0
# define STDOUT 1
# define STDERR 2
# define ERR_ARG "Wrong number of arguments\n"

typedef struct s_pipex
{
	int	fd1;
	int	fd2;
	char **paths; /**********malloc*********/
}		t_pipex;

// main.c
// main
void	open_files(char *file1, char *file2, t_pipex *pipex);
void	get_paths(char **envp, t_pipex *pipex);

// libft.c
void	ft_putstr_fd(int fd, char *str);
int		ft_strlen(char *str);
int		check_instr(const char *s1, const char *s2);
char	*ft_substr(char *s, size_t start, size_t len);

// ft_split.c
char	**ft_split(char const *s, char c);

#endif