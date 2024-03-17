/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 23:00:31 by caguillo          #+#    #+#             */
/*   Updated: 2024/03/17 01:27:12 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

// errno
# include <errno.h>
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
//# include "pipex.h"

# define STDIN 0
# define STDOUT 1
# define STDERR 2
# define USAGE_DOC "Usage: ./pipex here_doc LIMITER cmd cmd1 [cmd2 ... cmdn] OUTFILE\n"
# define USAGE "Usage: ./pipex INFILE cmd1 cmd2 [cmd3 ... cmdn] OUTFILE\n"
# define ERR_ARG "pipex: Wrong number of arguments\n"
# define ERR_CMD ": Command not found\n"
# define ERR_ACX ": Permission denied\n"
# define ERR_DIR ": No such file or directory\n"
# define ERR_GNL "pipex: Can't read input\n"
# define ERR_MAL "pipex: Malloc failed\n"
# define EXIT_SUCCESS 0
# define EXIT_FAILURE 1
# define EXIT_DENIED 126
# define EXIT_NOCMD 127
# define EXIT_NODIR 127

typedef struct s_pipex
{
	int		fd_in;
	int		fd_out;
	char	**paths;
	int		fd[2];
	int		status;
	pid_t	pid;
	int		is_heredoc;
	char	*lim;
	int		docfd[2];
}			t_pipex;

// main.c
// main
void		open_infile(char *infile, t_pipex *pipex);
void		open_outfile(char *outfile, t_pipex *pipex);
void		perror_open(t_pipex pipex, char *filename);
int			wait_exitcode(t_pipex pipex);

// void		children(t_pipex *pipex, char **argv, char **envp, int nbr_cmd);
void		child(t_pipex *pipex, char **argv, char **envp, int k);

// child.c
void		child_in(t_pipex *pipex, char **argv, char **envp, int k);
void		child(t_pipex *pipex, char **argv, char **envp, int k);
void		child_out(t_pipex *pipex, char **argv, char **envp, int k);
void		exec_arg(t_pipex pipex, char **argv, char **envp, int k);
void		exec_cmd(t_pipex pipex, char **argv, char **envp, int k);
void		exec_abs(t_pipex pipex, char **argv, char **envp, int k);

// path.c
void		get_paths(char **envp, t_pipex *pipex);
void		slash_paths(t_pipex *pipex);
char		*check_path(char **paths, char **cmd);
int			check_slash(char *str);
void		putstr_error(char *cmd0, char *err_str);

// free.c
void		close_exit(t_pipex pipex, int k);
void		perror_close_exit(char *err, t_pipex pipex, int k);
void		free_paths(t_pipex *pipex);
void		free_cmd(char **cmd);
void		free_close_exit(char **cmd, t_pipex *pipex, int exit_code,
				int is_paths);

// libft.c
void		ft_putstr_fd(char *str, int fd);
int			check_in_str(char *s1, char *s2);
char		*ft_strjoin(char *s1, char *s2);
char		*ft_strdup(char *s);
int			ft_strcmp(char *s1, char *s2);

// ft_split.c
char		**ft_split(char const *s, char c);

// heredoc.c
void		here_doc(t_pipex *pipex, char **argv, char **envp, int k);
void		fill_here_doc(t_pipex *pipex);

void		ft_putnbr_fd(int n, int fd);

#endif
