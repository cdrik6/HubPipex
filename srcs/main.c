/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 22:22:41 by caguillo          #+#    #+#             */
/*   Updated: 2024/03/11 01:32:37 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"
#include <errno.h>

int	main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;

	pipex = (t_pipex){0};
	if (argc != 5)
		return (ft_putstr_fd(ERR_ARG, 2), 1);
	//
	open_infile(argv[1], &pipex);         /****/
	open_outfile(argv[argc - 1], &pipex); /*********/
	//
	// d = dup2(pipex.fd_in, STDIN);
	// close(pipex.fd_in);
	// if (d == -1)
	// 	return (perror("dup2 infile"), close(pipex.fd_out), 1);
	//
	// d = dup2(pipex.fd_out, STDOUT);
	// close(pipex.fd_out);
	// if (d == -1)
	// 	return (perror("dup2 outfile"), 1);
	//
	if (pipe(pipex.fd) == -1)
		perror_close_exit("pipe", pipex, 1);
	child_in(&pipex, argv, envp);
	child_out(&pipex, argv, envp);
	return (wait_exitcode(pipex));
}

// while (waitpid(-1, NULL, 0) != -1)
int	wait_exitcode(t_pipex pipex)
{
	int	exitcode;

	while (errno != ECHILD)
	{
		if (wait(&pipex.status) == pipex.pid)
		{
			if (WIFEXITED(pipex.status))
				exitcode = WEXITSTATUS(pipex.status);
		}
	}
	if (pipex.w_out == -1)
		exitcode = 1;
	return (exitcode);
}

void	open_infile(char *file1, t_pipex *pipex)
{
	if (access(file1, R_OK) == 0)
	{
		(*pipex).fd_in = open(file1, O_RDONLY);
		if ((*pipex).fd_in < 0)
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
		(*pipex).fd_out = open(file2, O_TRUNC | O_CREAT | O_RDWR, 0644);
		if ((*pipex).fd_out < 0)
		{
			perror("open outfile");
			close((*pipex).fd_in);
			exit(EXIT_FAILURE);
		}
	}
	else if (access(file2, W_OK) == 0)
	{
		(*pipex).fd_out = open(file2, O_TRUNC | O_CREAT | O_RDWR, 0644);
		if ((*pipex).fd_out < 0)
		{
			perror("open outfile");
			close((*pipex).fd_in);
			exit(EXIT_FAILURE);
		}
		// close((*pipex).fd1);
		// close((*pipex).fd2);
		// exit(EXIT_DENIED);
	}
	else
	{
		perror("outfile access");
		(*pipex).w_out = -1;
		//(*pipex).fd2 = open(file2, O_RDONLY);
		// ft_putnbr_fd((*pipex).fd2, 2);
	}
}
