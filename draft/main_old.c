/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 22:22:41 by caguillo          #+#    #+#             */
/*   Updated: 2024/03/09 01:06:40 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

int	main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;
	int		d;

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
	if (pipe(pipex.fd) == -1)
		perror_close_exit("pipe", pipex, 1);
	//
	fork_child(pipex, argv, envp, 1);
	// fork_child(pipex, argv, envp, 2);
	// exec_arg(pipex, argv, envp, 2);
	// wait(&(pipex.status));
	// waitpid(-1, NULL, WNOHANG);
	// ft_putnbr_fd(pipex.status[1], 2);
	// ft_putnbr_fd(pipex.status[0], 2);
	// exit(pipex.status[1]);
	return (0);
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

// void	open_files(char *file1, char *file2, t_pipex *pipex)
// {
// 	if (access(file1, R_OK) == 0)
// 	{
// 		(*pipex).fd1 = open(file1, O_RDONLY);
// 		if ((*pipex).fd1 < 0)
// 		{
// 			perror("open infile");
// 			exit(EXIT_FAILURE);
// 		}
// 	}
// 	else
// 	{
// 		perror("access");
// 		ft_putstr_fd(2, ERR_ACC);
// 		exit(EXIT_FAILURE);
// 	}
// 	(*pipex).fd2 = open(file2, O_TRUNC | O_CREAT | O_RDWR, 0644);
// 	if ((*pipex).fd2 < 0)
// 	{
// 		perror("open outfile");
// 		close((*pipex).fd1);
// 		exit(EXIT_FAILURE);
// 	}
// 	if (access(file2, W_OK) != 0)
// 	{
// 		perror("access");
// 		ft_putstr_fd(2, ERR_ACC);
// 		close((*pipex).fd1);
// 		close((*pipex).fd2);
// 		exit(EXIT_FAILURE);
// 	}
// }
