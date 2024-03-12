/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 22:22:41 by caguillo          #+#    #+#             */
/*   Updated: 2024/03/12 02:08:53 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"
#include <bits/fcntl-linux.h>
#include <errno.h>

int	main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;

	pipex = (t_pipex){0};
	if (argc != 5)
		return (ft_putstr_fd(ERR_ARG, 2), 1);
	//
	if (pipe(pipex.fd) == -1)
		perror_close_exit("pipex: pipe", pipex, 1);
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
	return (exitcode);
}

// if exist and readable --> open
// else exit (from child)
void	open_infile(char *infile, t_pipex *pipex)
{
	char	*tmp;

	if (access(infile, R_OK) == 0)
	{
		(*pipex).fd_in = open(infile, O_RDONLY);
		if ((*pipex).fd_in < 0)
		{
			tmp = ft_strjoin("pipex: ", infile);
			perror(tmp);
			free(tmp);
			close((*pipex).fd[0]);
			close((*pipex).fd[1]);
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		tmp = ft_strjoin("pipex: ", infile);
		perror(tmp);
		free(tmp);
		close((*pipex).fd[0]);
		close((*pipex).fd[1]);
		exit(EXIT_FAILURE);
	}
}

// if don't exist --> open/create en write
// if exist in write mode --> open/trunc
// if write not possible--> error + exit
void	open_outfile(char *outfile, t_pipex *pipex)
{
	char	*tmp;

	(*pipex).fd_out = open(outfile, O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if ((*pipex).fd_out < 0)
	{
		tmp = ft_strjoin("pipex: ", outfile);
		perror(tmp);
		free(tmp);		
		close((*pipex).fd[0]);
		close((*pipex).fd[1]);
		exit(EXIT_FAILURE);
	}
}

// // if don't exist --> open/create
// // elseif exist in write mode --> open/trunc
// // else exist and no-write --> error
// void	open_outfile(char *outfile, t_pipex *pipex)
// {
// 	if (access(outfile, F_OK) != 0)
// 	{
// 		(*pipex).fd_out = open(outfile, O_TRUNC | O_CREAT | O_RDWR, 0644);
// 		if ((*pipex).fd_out < 0)
// 		{
// 			ft_putstr_fd("pipex: ", 2);
// 			perror(outfile);
// 			// close((*pipex).fd_in);
// 			//(*pipex).w_out = -1;
// 			close((*pipex).fd[0]);
// 			close((*pipex).fd[1]);
// 			exit(EXIT_FAILURE);
// 		}
// 	}
// 	else if (access(outfile, W_OK) == 0)
// 	{
// 		(*pipex).fd_out = open(outfile, O_TRUNC | O_CREAT | O_RDWR, 0644);
// 		if ((*pipex).fd_out < 0)
// 		{
// 			ft_putstr_fd("pipex: ", 2);
// 			perror(outfile);
// 			// close((*pipex).fd_in);
// 			//(*pipex).w_out = -1;
// 			close((*pipex).fd[0]);
// 			close((*pipex).fd[1]);
// 			exit(EXIT_FAILURE);
// 		}
// 	}
// 	else
// 	{
// 		ft_putstr_fd("pipex: ", 2);
// 		perror(outfile);
// 		//(*pipex).w_out = -1;
// 		close((*pipex).fd[0]);
// 		close((*pipex).fd[1]);
// 		exit(EXIT_FAILURE);
// 	}
// }
