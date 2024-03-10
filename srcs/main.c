/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 22:22:41 by caguillo          #+#    #+#             */
/*   Updated: 2024/03/10 01:21:32 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"
#include <errno.h>

int	main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;
	int		d;
	int		k;

	// pid_t	pid;
	pipex = (t_pipex){0};
	if (argc != 5)
		return (ft_putstr_fd(2, ERR_ARG), 1);
	//
	open_infile(argv[1], &pipex);
	open_outfile(argv[argc - 1], &pipex);
	//
	d = dup2(pipex.fd1, STDIN);
	close(pipex.fd1);
	if (d == -1)
		return (perror("dup2 infile"), close(pipex.fd2), 1);
	d = dup2(pipex.fd2, STDOUT);
	close(pipex.fd2);
	if (d == -1)
		return (perror("dup2 outfile"), 1);
	//
	//
	// if (pipe(pipex.fd) == -1)
	// 		perror_close_exit("pipe", pipex, 1);
	// fork_child(pipex, argv, envp, 1);
	//
	if (pipe(pipex.fd) == -1)
			perror_close_exit("pipe", pipex, 1);
	k = 0;
	while (k < 2)
	{
		
		pipex.pid = fork();
		if (pipex.pid == -1)
			perror_close_exit("fork", pipex, 1);
		if (pipex.pid == 0)
		{
			close(pipex.fd[0]);
			dup2(pipex.fd[1], STDOUT);
			close(pipex.fd[1]);
			exec_arg(pipex, argv, envp, k);
		}
		close(pipex.fd[1]);
		dup2(pipex.fd[0], STDIN);
		close(pipex.fd[0]);
	}
	// while(wait(&pipex.status[1]) != -1)
	//while (waitpid(-1, NULL, 0) != -1)
	// while (errno != ECHILD)
	// 	wait(&pipex.status[1]);
	int ret;
	while (errno != ECHILD)
	{
		if (wait(&pipex.status[1]) == pipex.pid)
		{
			if (WIFEXITED(pipex.status[1]))
				ret = WEXITSTATUS(pipex.status[1]);
			else
				ret = 128 + WTERMSIG(pipex.status[1]);
		}
	}
	return (ret);
}

void	open_infile(char *file1, t_pipex *pipex)
{
	if (access(file1, R_OK) == 0)
	{
		(*pipex).fd1 = open(file1, O_RDONLY);
		if ((*pipex).fd1 < 0)
		{
			perror("open infile");
			exit(EXIT_FAILURE);
		}
	}
	else
		perror("infile access");
}

// else exist and no-write
void	open_outfile(char *file2, t_pipex *pipex)
{
	if (access(file2, F_OK) != 0)
	{
		(*pipex).fd2 = open(file2, O_TRUNC | O_CREAT | O_RDWR, 0644);
		if ((*pipex).fd2 < 0)
		{
			perror("open outfile");
			close((*pipex).fd1);
			exit(EXIT_FAILURE);
		}
	}
	else if (access(file2, W_OK) == 0)
	{
		(*pipex).fd2 = open(file2, O_TRUNC | O_CREAT | O_RDWR, 0644);
		if ((*pipex).fd2 < 0)
		{
			perror("open outfile");
			close((*pipex).fd1);
			exit(EXIT_FAILURE);
		}
		// close((*pipex).fd1);
		// close((*pipex).fd2);
		// exit(EXIT_DENIED);
	}
	else
	{
		perror("outfile access");
		(*pipex).err_outf = 1;
		//(*pipex).fd2 = open(file2, O_RDONLY);
		// ft_putnbr_fd((*pipex).fd2, 2);
	}
}
