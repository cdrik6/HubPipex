/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 22:30:23 by caguillo          #+#    #+#             */
/*   Updated: 2024/03/06 20:02:42 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

//
//# include <errno.h>
//
# include <fcntl.h>
// perror
# include <stdio.h>
// exit code
# include <stdlib.h>
// waitpid
# include <sys/wait.h>
// write, execve, pipe, close
# include <unistd.h>
// gnl
# include "get_next_line.h"

# define STDIN 0
# define STDOUT 1
# define STDERR 2
# define ERR_ARG "Wrong number of arguments\n"
# define ERR_CMD "Command not found\n"
# define ERR_ACC "Permission denied\n"

typedef struct s_pipex
{
	int		fd1;
	int		fd2;
	char	**paths;
	int		fd[2];
	//int		status;
}			t_pipex;

// main.c
// main
void		open_files(char *file1, char *file2, t_pipex *pipex);
void		fork_child(t_pipex pipex, char **argv, char **envp, int k);
void		exec_cmd(t_pipex pipex, char **argv, char **envp, int k);

// path.c
void		get_paths(char **envp, t_pipex *pipex);
void		slash_paths(t_pipex *pipex);
char		*check_path(char **paths, char **cmd);
// void		get_cmds(char **argv, t_pipex *pipex);

// free.c
void		close_exit(t_pipex pipex, int k);
// void		close_pipe(t_pipex pipex);
void		free_paths(t_pipex *pipex);
void		free_cmd(char **cmd);

// libft.c
void		ft_putstr_fd(int fd, char *str);
int			check_in_str(char *s1, char *s2);
char		*ft_strjoin(char *s1, char *s2);
char		*ft_strdup(char *s);

// ft_split.c
char		**ft_split(char const *s, char c);

#endif
