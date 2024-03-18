/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 19:28:23 by caguillo          #+#    #+#             */
/*   Updated: 2024/03/18 01:45:40 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	child_in(t_pipex *pipex, char **argv, char **envp, int k)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		perror_close_exit("pipex: fork", *pipex, EXIT_FAILURE);
	if (pid == 0)
		child_in_child(pipex, argv, envp, k);
	if ((*pipex).is_heredoc == 1)
		close((*pipex).docfd[0]);
	close((*pipex).fd[1]);
	if (dup2((*pipex).fd[0], STDIN) == -1)
		perror_close_exit("pipex: dup2", *pipex, EXIT_FAILURE);
	close((*pipex).fd[0]);
}

void	child_in_child(t_pipex *pipex, char **argv, char **envp, int k)
{
	close((*pipex).fd[0]);
	if ((*pipex).is_heredoc == 1)
	{
		if (dup2((*pipex).docfd[0], STDIN) == -1)
			perror_close_exit("pipex: dup2", *pipex, EXIT_FAILURE);
		close((*pipex).docfd[0]);
	}
	else
	{
		open_infile(argv[1], pipex);
		if (dup2((*pipex).fd_in, STDIN) == -1)
			perror_close_exit("pipex: dup2", *pipex, EXIT_FAILURE);
		close((*pipex).fd_in);
	}
	if (dup2((*pipex).fd[1], STDOUT) == -1)
		perror_close_exit("pipex: dup2", *pipex, EXIT_FAILURE);
	close((*pipex).fd[1]);
	exec_arg((*pipex), argv, envp, k);
}

void	child(t_pipex *pipex, char **argv, char **envp, int k)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		perror_close_exit("pipex: fork", *pipex, 1);
	if (pid == 0)
	{
		close((*pipex).fd[0]);
		if (dup2((*pipex).fd[1], STDOUT) == -1)
			perror_close_exit("pipex: dup2", *pipex, EXIT_FAILURE);
		close((*pipex).fd[1]);
		exec_arg((*pipex), argv, envp, k);
	}
	close((*pipex).fd[1]);
	if (dup2((*pipex).fd[0], STDIN) == -1)
		perror_close_exit("pipex: dup2", *pipex, EXIT_FAILURE);
	close((*pipex).fd[0]);
}

void	child_out(t_pipex *pipex, char **argv, char **envp, int k)
{
	(*pipex).pid = fork();
	if ((*pipex).pid == -1)
		perror_close_exit("pipex: fork", (*pipex), 1);
	if ((*pipex).pid == 0)
	{
		close((*pipex).fd[1]);
		close((*pipex).fd[0]);
		open_outfile(argv[k + 1], pipex);
		if (dup2((*pipex).fd_out, STDOUT) == -1)
			perror_close_exit("pipex: dup2", *pipex, EXIT_FAILURE);
		close((*pipex).fd_out);
		exec_arg((*pipex), argv, envp, k);
	}
	close((*pipex).fd[1]);
	if (dup2((*pipex).fd[0], STDIN) == -1)
		perror_close_exit("pipex: dup2", *pipex, EXIT_FAILURE);
	close((*pipex).fd[0]);
}

void	exec_arg(t_pipex pipex, char **argv, char **envp, int k)
{
	if (check_slash(argv[k]) == 1)
		exec_abs(pipex, argv, envp, k);
	else
		exec_cmd(pipex, argv, envp, k);
}
